#
#For Congressional Coding App Challenge
#
#Overview:
#    Makes a neural network with three hidden layers
#    the network is then given 8000 training cases
#    where it can learn to guess how to play poker
#    from here a formatted hand is given to the program.
#    the program then returns its choice,
#    and is then ready for the next input.
#    If the AI decides to raise, then a linear function is
#    applied that is proportional to its hand rank and gives
#    a floating point number in return.
#
#    To Do:
#       Get with Smedley and work on interprogram communications
#Last worked on:
#    10/26/26


import random
import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from phevaluator.evaluator import evaluate_cards
import getpass
import time


# dict to convert cards into numbers for the AI
RANK_MAP = {str(i): i for i in range(2, 10)}
RANK_MAP.update({'T': 10, 'J': 11, 'Q': 12, 'K': 13, 'A': 14})
SUIT_MAP = {'S': 1, 'H': 2, 'D': 3, 'C': 4}

# actions the AI can choose from
ACTIONS = ["FOLD", "CALL", "RAISE"]

#file initialization
username = getpass.getuser()
username = username.lower()
filepath = r"C:\Users"
filepath = filepath + "\\" +username + "\\" + "text.txt"
try:
    with open(filepath, "x") as file:
        content = file.read()
except FileExistsError:
    print("File already exists. Skipping...")


#makes the training data for the AI, along eval values
def generate_valid_poker_training_data(num_samples):
    #makes a training deck
    deck = [f"{r}{s}" for r in RANK_MAP.keys() for s in SUIT_MAP.keys()]

    card_features_scaled = np.zeros((num_samples, 14))
    hand_strength_scaled = np.zeros((num_samples, 1))

    #print("Evaluating actual hand strengths and generating padded states...")

    success_count = 0
    while success_count < num_samples:
        # draws 7 unique cards to establish the true final hand strength label
        row_cards = random.sample(deck, 7)

        try:
            # evaluates using 7 cards 1 = best, 7462 = worst
            score = evaluate_cards(*row_cards)

            # randomly choose game state for data variety ie flop, turn, river
            game_state = random.choice([3, 4, 5])
            visible_cards = row_cards[:2] + row_cards[2:2 + game_state]

            # map known cards to a scaled grid
            numeric_row = []
            for card in visible_cards:
                r_part = card[:-1]
                s_part = card[-1]
                numeric_row.append(float(RANK_MAP[r_part]) / 14.0)
                numeric_row.append(float(SUIT_MAP[s_part]) / 4.0)

            while len(numeric_row) < 14:
                numeric_row.append(0.0)

            card_features_scaled[success_count] = numeric_row

            #scales the poker hand down to a floating int from 1.0 to ~0.0, to make it easier for the AI to understand
            hand_strength_scaled[success_count] = [float(score) / 7462.0]

            success_count += 1

        except Exception:
            continue

    return card_features_scaled, hand_strength_scaled


def parse_cards_with_padding(cards_str, expected_count):
    #makes a list of cards(value,suit), with number values
    #will always return five cards, if there isn't enough, padding in the form of 0,0 will be added
    cards = cards_str.strip().upper().split(' ')
    cards = [c for c in cards if c]

    if len(cards) > expected_count:
        raise ValueError(f"Too many cards provided. Expected maximum of {expected_count}.")

    numeric_features = []
    for card in cards:
        if len(card) < 2:
            raise ValueError(f"Invalid card format: {card}")

        rank_part = card[:-1]
        suit_part = card[-1]

        if rank_part not in RANK_MAP or suit_part not in SUIT_MAP:
            raise ValueError(f"Unknown card Rank or Suit: {card}")

        numeric_features.append(float(RANK_MAP[rank_part]) / 14.0)
        numeric_features.append(float(SUIT_MAP[suit_part]) / 4.0)

    while len(numeric_features) < (expected_count * 2):
        numeric_features.append(0.0)

    return numeric_features

def main():

    # ==========================================================
    # 1. GENERATE TRAINING DATA
    # ==========================================================
    #high number increases training stability
    num_samples = 8000
    card_features_scaled, hand_strength_scaled = generate_valid_poker_training_data(num_samples)

    max_bet = 500.0
    max_money = 5000.0

    raw_external_bets = np.random.randint(10, 501, size=(num_samples, 1))
    raw_ai_money = np.random.randint(100, 5001, size=(num_samples, 1))

    scaled_external_bets = raw_external_bets / max_bet
    scaled_ai_money = raw_ai_money / max_money

    # 8000 is the amount of training data
    # 17 is the amount of input data needed for an accurate output
    x_train = np.hstack((card_features_scaled, hand_strength_scaled, scaled_external_bets, scaled_ai_money))

    # Target calculations use unscaled score (reconstructed by multiplying scaled input back by 7462)

    #a little bit obscure but the hand strength scaled goes back to the gen training data function that
    #uses perfect hash to evaluate each hande
    #it gets unscaled from ~0.0 to 1.0 --> 1 to 7462

    unscaled_scores = (hand_strength_scaled * 7462.0).flatten()
    bet_ratio = raw_external_bets.flatten() / (raw_ai_money.flatten() + 1)

    y_train = []
    for i in range(num_samples):
        decision_metric = unscaled_scores[i] + (bet_ratio[i] * 3500)
        if decision_metric > 5800:
            y_train.append(0)  # fold
        elif decision_metric > 3100:
            y_train.append(1)  # call
        else:
            y_train.append(2)  # raise

    y_train = np.array(y_train)

    classes, counts = np.unique(y_train, return_counts=True)
    total_samples = len(y_train)
    class_weight_dict = {cls: total_samples / (len(classes) * count) for cls, count in zip(classes, counts)}

    # ==========================================================
    # 2. BUILD MODEL 17 NODE ARCHITECTURE
    # ==========================================================
    model = models.Sequential([
        # 17 nodes because there is 14 card inputs, 1 hand eval score and 2 betting inputs
        layers.Input(shape=(17,)),

        layers.Dense(256, activation='relu'),
        layers.BatchNormalization(),
        layers.Dropout(0.2),

        layers.Dense(128, activation='relu'),
        layers.BatchNormalization(),
        layers.Dropout(0.2),

        layers.Dense(64, activation='relu'),
        layers.Dense(3, activation='softmax')
    ])

    model.compile(
        optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),
        loss='sparse_categorical_crossentropy',
        metrics=['accuracy']
    )

    # ==========================================================
    # 3. TRAIN THE AI
    # ==========================================================
    print("\n--- STARTING TRAINING WITH HAND SCORE INJECTION ---")
    model.fit(
        x_train, y_train,
        epochs=35,
        batch_size=64,
        validation_split=0.15,
        class_weight=class_weight_dict,
        verbose=1
    )
    print("\nTraining complete! Accuracy optimized successfully.")

    # ==========================================================
    # 4. USER INPUT LOOP
    # ==========================================================
    while True:
        '''
        print("\n--- Hand-Feed Data to the Poker AI ---")
        print("Required format: [2 AI Cards], [Up to 5 Board Cards], [external bet], [ai money]")
        print("Example (Flop):  KH KS, 2D TC JD, 150, 1000")
        print("Example (River): KH KS, 2D TC JD QS 3H, 150, 1000")
        '''

        with open(filepath, "r") as file:
            contents = file.read()
        user_input = contents
        '''
        with open(filepath, "w") as file:
            file.write("")
        '''
        if user_input.lower() == 'exit':
            print("Goodbye!")
            break

        try:
            parts = user_input.split(',')
            if len(parts) != 4:
                time.sleep(2)
                #raise ValueError("Input must have exactly 3 commas separating the 4 sections.")



            # Isolate raw card string chunks cleanly
            ai_raw_str = parts[0].strip().upper()
            board_raw_str = parts[1].strip().upper()

            # Merge strings to calculate the current known hand cards
            combined_cards_list = (ai_raw_str + " " + board_raw_str).split()

            #makes a list of the combined cards to be evaluated
            eval_cards = list(combined_cards_list)
            # makes a fresh, clean card deck copy to draw from safely
            deck_filler = [f"{r}{s}" for r in RANK_MAP.keys() for s in SUIT_MAP.keys()]
            # removes cards already held in the hand from the pool of cards
            deck_filler = [card for card in deck_filler if card not in eval_cards]

            #fills the unused cards with 00 until there is zero cards remaining
            while len(eval_cards) < 7:
                chosen_filler = random.choice(deck_filler)
                deck_filler.remove(chosen_filler)
                eval_cards.append(chosen_filler)

            #calulates a score that's between ~0.0 and 1.0 so that it is easier on the neurons
            live_hand_score = float(evaluate_cards(*eval_cards)) / 7462.0

            #makes the input easier to understand for the network
            ai_card_numerical = parse_cards_with_padding(parts[0], expected_count=2)
            board_card_numerical = parse_cards_with_padding(parts[1], expected_count=5)
            val_external_bets = float(parts[2].strip()) / max_bet
            val_ai_money = float(parts[3].strip()) / max_money

            #makes a 17 node vector
            final_features = ai_card_numerical + board_card_numerical + [live_hand_score] + [val_external_bets, val_ai_money]
            features_array = np.array([final_features])

            probabilities = model.predict(features_array, verbose=0)[0]

            chosen_action_idx = np.argmax(probabilities)
            chosen_action = ACTIONS[chosen_action_idx]
            confidence = probabilities[chosen_action_idx] * 100
            print("\nAI DECISION ANALYSIS:")
            print(f" -> Calculated Hand Value Rank: {int(live_hand_score * 7462)} / 7462")
            print(f" -> Primary Choice: **{chosen_action}** (Confidence: {confidence:.2f}%)")
            if chosen_action == "RAISE":
                with open(filepath, "w") as file:
                    file.write("RAISE")
                    file.write(str(int(-0.25*(live_hand_score*7462) + float(val_ai_money*max_money))))
            elif chosen_action == "CALL":
                with open(filepath, "w") as file:
                    file.write("CALL")
            else:
                with open(filepath, "w") as file:
                    file.write("FOLD")


            #print(f" -> Breakdown: [FOLD: {probabilities[0] * 100:.1f}%] | [CALL: {probabilities[1] * 100:.1f}%] | [RAISE: {probabilities[2] * 100:.1f}%]")


        except ValueError as e:
            print(f" Input Error: {e}")
        except Exception as e:
            print(f" Execution Error: {e}")


if __name__ == '__main__':
    main()
