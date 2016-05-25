// Spencer Kim
// May 25 2016

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

struct Card {
	int suit; // 0 to 3
	int num; // 0 to 12; 2 to A
	Card(int suit_, int num_) : suit(suit_), num(num_) { }
};

void shuffle(vector<Card> &deck);
void print_hand(const vector<Card> &hand, const string& msg);


int main(int argc, char** argv) {
	if(argc != 3){
		cerr << "Usage: ./bspoker hand_size num_rounds\n";
		exit(0);
	}
	const int hand_size_k = atoi(argv[1]);
	const int num_rounds_k = atoi(argv[2]);
	const bool print_hands = false;

	// if(hand_size_k < 5){
	// 	cerr << "Error: Hand size must be at least 5\n";
	// 	exit(0);	
	// }

	// generate 52 card deck
	vector<Card> deck;
	for(int i=0; i<4; i++){
		for(int j=0; j<13; j++){
			if(j == 0){
				deck.emplace_back(4, j); // wild has no suit
			} else {
				deck.emplace_back(i, j);
			}
		}
	}

	map<string, int> counts;

	for(int i=0; i<num_rounds_k; i++){
		// shuffle 7 times obv
		for(int i=0; i<7; i++){
			shuffle(deck);
		}
		
		vector<Card> hand(deck.begin(), deck.begin()+hand_size_k);

		// sort the hand; also used for straights
		sort(hand.begin(), hand.end(), [](auto &left, auto &right) {
			return left.num < right.num;
		});

		if(print_hands) print_hand(hand, "Hand");

		// counts used for pair, trip, full house, flush, etc. 
		unordered_map<int, int> hand_counts; 
		unordered_map<int, int> suit_counts; 
		for(auto card : hand){
			hand_counts[card.num]++;
			suit_counts[card.suit]++;
		}
		// capture # of wildcards, and remove it from hand counts
		int num_wilds = hand_counts[0];
		hand_counts.erase(0);
		suit_counts.erase(4);


		// TODO: remove copy pasta code

		

		// pair
		for(auto pair : hand_counts) {
			if(pair.second + num_wilds >= 2){
				counts["1. Pair"]++;
				break;
			}
		}

		// triple
		for(auto pair : hand_counts) {
			if(pair.second + num_wilds >= 3){
				counts["2. Triple"]++;
				break;
			}
		}

		// four of a kind
		for(auto pair : hand_counts) {
			if(pair.second + num_wilds >= 4){
				counts["6. Quad"]++;
				break;
			}
		}

		// five of a kind
		for(auto pair : hand_counts) {
			if(pair.second + num_wilds >= 5){
				counts["8. Five of a kind"]++;
				break;
			}
		}

		// six of a kind
		for(auto pair : hand_counts) {
			if(pair.second + num_wilds >= 6){
				counts["9a. Six of a kind"]++;
				break;
			}
		}

		// seven of a kind
		for(auto pair : hand_counts) {
			if(pair.second + num_wilds >= 7){
				counts["9b. Seven of a kind"]++;
				// TODO: make this debug output
				// print_hand(hand, "7kind");
				break;
			}
		}

		// eight of a kind
		for(auto pair : hand_counts) {
			if(pair.second + num_wilds >= 8){
				counts["9c. Eight of a kind"]++;
				break;
			}
		}

		// flush
		for(auto pair : suit_counts) {
			if(pair.second + num_wilds >= 5){
				counts["4. Flush"]++;
				// print_hand(hand, "flush");
				break;
			}
		}

		// full house
	if(hand_size_k >= 5){
		if(num_wilds == 0) {
			bool triple_found = false, pair_found = false;
			for(auto pair : hand_counts) {
				if(pair.second >= 3) {
					if(pair_found) {
						counts["5. Full House"]++;
						break;
					} else{
						triple_found = true;
					}
				}
				else if(pair.second >= 2) {
					if(triple_found) {
						counts["5. Full House"]++;
						break;
					} else{
						pair_found = true;
					}
				}
			}
		} else if (num_wilds == 1) {
			// check for triple or 2 pairs
			bool pair_found = false;
			for(auto pair : hand_counts) {
				if(pair.second >= 3){
					counts["5. Full House"]++;
					break;
				}
				else if(pair.second >= 2) {
					if(pair_found) {
						counts["5. Full House"]++;
						break;
					} else {
						pair_found = true;
					}
				}
			}
		} else if (num_wilds == 2) {
			// check for pair
			for(auto pair : hand_counts) {
				if(pair.second >= 2){ 
					counts["5. Full House"]++;
					break;
				}
			}
		} else { 
			// can make full house with any hand
			counts["5. Full House"]++;
		}
	}

		// straight
		for(int low_card = 1, high_card = 5; high_card <= 12; low_card++, high_card++){
			int straight_count = num_wilds;
			for(int i=low_card; i<=high_card; i++){
				if(hand_counts.find(i) != hand_counts.end()){
					straight_count++;
				}
			}
			if(straight_count >= 5){
				counts["3. Straight"]++;
				break;
			}
		}

		// straight flush
		for(int suit = 0; suit < 3; suit++) {
			// make a hand for each suit and check for straight
			bool straight_flush_found = false;
			set<int> tmp_hand;
			for(auto card : hand){
				if(card.suit == suit){
					tmp_hand.insert(card.num);
				}
			}

			// copy pasta "straight" code cuz this is a hack
			for(int low_card = 1, high_card = 5; high_card <= 12; low_card++, high_card++){
				int straight_count = num_wilds;
				for(int i=low_card; i<=high_card; i++){
					if(tmp_hand.find(i) != tmp_hand.end()){
						straight_count++;
					}
				}
				if(straight_count >= 5){
					counts["7. Straight Flush"]++;
					// print_hand(hand, "Straight Flush");
					straight_flush_found = true;
					break;
				}
			}
			
			if(straight_flush_found) break;
		}

	}
	cout << "Data:\n";
	for(auto pair : counts) {
		cout << pair.first << ": " << pair.second << endl;
	}

	return 0;
}

void shuffle(vector<Card> &deck){
	int n = deck.size();
	mt19937 gen (chrono::system_clock::now().time_since_epoch().count());

	for(int i=n-1; i>0; i--){
		int j = gen() % (i+1);
		swap(deck[i], deck[j]);
	}
}

void print_hand(const vector<Card> &hand, const string& msg){
	// const static string suit_mapping[] = {"\xe2\x99\xa3", "\xe2\x99\xa5", "\xe2\x99\xa6", "\xe2\x99\xa0"};
	const static string suit_mapping[] = {"\xe2\x99\xa3", "\xe2\x99\xa2", "\xe2\x99\xa1", "\xe2\x99\xa0"};
	const static string num_mapping[] = {"WILD", "3", "4", "5", "6", 
										"7", "8", "9", "10", "J", "Q", "K", "A"};

	cout << msg << ": ";
	for(auto card : hand){
		cout << num_mapping[card.num] << "" << suit_mapping[card.suit] << ", ";
	}
	cout << endl;
}

