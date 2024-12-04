#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>

using namespace std;

class supermarket{

	private:
		fstream inventory;
		fstream sales;
		string productName;
		string productCode;
		float productPrice;
		map<string, int> basketCodeArr;
		map<string, float> discounts;

	public:
		void menu(){

			x:
			string email;
			string password;

			cout<<"\nMenu!";
			cout<<"\n\t [1] Admin"
				<<"\n\t [2] Customer"
				<<"\n\t [3] Exit";
			cout<<"\nPlease select an option: ";
			int s;
			cin>>s;

			switch(s){
				case 1:
					cout<<"\n\tAdmin Login \t(Enter 'q' for both fields to go back to the main menu)\n";
					y:
					cout<<"\t\tEnter email: ";
					cin>>email;
					cout<<"\t\tEnter password: ";
					cin>>password;
					
					if (email == "admin@gmail.com" && password == "HireMe!")
					{
						administrator();
						//clear credentials when admin logs out 
						email = "";
						password = "";
					} else if (email == "q" && password == "q") {
						break;
					} else {
						cout<<"\n\t\tInvalid email or password.\n";
						goto y;
					}
					break;
				case 2:
					customer();
					break;
				case 3:
					cout<< "\nGoodbye! Exiting.";
					exit(0);
				default:
					//fix this to loop
					cout<< "\nInvalid choice.";
					break;
			}
			goto x;

		};

		//If user takes admin role, they are prompted with new choices that control the system
		void administrator(){
			z:
			cout<<"\t\tAdmin Menu\n";
			cout<<"\t\t\t [1] Add inventory items\n"
				<<"\t\t\t [2] Remove inventory item\n"
				<<"\t\t\t [3] Define a sale set\n"
				<<"\t\t\t [4] Remove a sale set\n"
				<<"\t\t\t [5] Log out\n";

			cout<<"Please select an option: ";
			int s;
			cin>>s;

			switch(s){
				case 1:
					addInventory();
					break;
				case 2:
					removeInventory();
					break;
				case 3:
					defineSet();
					break;
				case 4:
					removeSet();
					break;
				case 5:
					cout<< "Logging out of admin!\n";
					return;
				default:
					cout<< "Invalid choice.\n";
					break;
			}
			goto z;
		}

		//If user selects customer role, they begin scanning items and a receipt prints when they're done
		void customer(){
			m:
			int c = 0;

			cout<<"\n____________________________________________________________________________________"
				<<"\n|  \t\t\t\t Start Scanning Items \t\t\t\t   |"
				<<"\n|__________________________________________________________________________________|"
				<<"\nEnter product codes. When you're finished, enter 'done': \n";

			string input;
			string endKey = "done";
			while(true) {
				cin>>input;
				if(input==endKey){ //user wants to quit
					break;
				}
				//user has entered a product code
				if(basketCodeArr.find(input)==basketCodeArr.end()){ //the code is not yet in their "basket"
					basketCodeArr[input]=1; //add code with quantity 1

				} else { //the code is in their basket, incrementing quantity
					basketCodeArr[input] ++;
				}
				
				c++;
			}

			receipt();
			return;
		}

		//An option in admin role, writes a line of product information to "supermarket_inventory.txt"
		void addInventory(){

			inventory.open("supermarket_inventory.txt", ios::in);

			cout<<"\t\t\t Add Inventory\n";
			x:
			cout<<"\t\t\t\t Enter Product Code: ";
			cin>>productCode;
			cout<<"\t\t\t\t Enter Product Name (use '_' for spaces): ";
			cin>>productName;
			cout<<"\t\t\t\t Enter Product Price: ";
			cin>>productPrice;

			
			if (!inventory){
				inventory.open("supermarket_inventory.txt", ios::app|ios::out);
				inventory<<productCode<<" "<<productName<<" "<<productPrice <<"\n";
				cout<<"\t\t\t\t Successfully added!\n";
			} else {
				string c;
				string n;
				float p;
				bool present;

				inventory>>c>>n>>p;
				while(!inventory.eof()){
					if(c==productCode){
						present = true;
						cout<<"Product code was already in inventory.\n";
						break;
					}
					inventory>>c>>n>>p;
					present=false;
				}
				if (!present){
					inventory.close();
					inventory.open("supermarket_inventory.txt", ios::app|ios::out);
					inventory<<productCode<<" "<<productName<<" "<<productPrice<<"\n";
					cout<<"\t\t\t\t Successfully added!\n";
				}

			}

			//prompt user if they want to add another
			char choice;
			cout<<"\t\t\t Add another? (y/n)";
			cin>>choice;
			if(choice=='y'){ //loop if they input 'y'
				goto x; 
			} else { //any other input returns to admin menu
				inventory.close(); 
				return; 
			}

		}

		//An option in admin role, removes a line from "supermarket_inventory.txt" based on indicated product code
		void removeInventory(){
			cout<<"\t\t\t Remove Inventory\n";
			inventory.open("supermarket_inventory.txt", ios::in);
			if (!inventory){
				cout<<"Failed to open inventory!\n";
				return;
			}
			fstream data;
			data.open("temp.txt", ios::out);

			cout<<"\t\t\t\t Enter a product code to be removed: ";
			string input;
			cin>>input;

			string c, n;
			float p;
			bool present = false;
			while(inventory>>c>>n>>p){
				if(c!=input){ //this line is not the product that the user wishes to remove
					data<<c<<" "<<n<<" "<<p<<"\n"; //write the full line to the new file
				} else {present=true;}
			}

			inventory.close();
			data.close();

			if(present){
				//delete the old file and rename new file (missing the desired product)
				remove("supermarket_inventory.txt");
				rename("temp.txt", "supermarket_inventory.txt");
				cout<<"Removed!\n";
			} else {
				remove("temp.txt");
				cout<<"Product was not found.\n";
			}
			

			return;

		}

		//admin menu option that adds a line to "supermarket_sale_sets.txt" of three product codes
		void defineSet(){
			cout<<"\t\t\t Add Sale Set\n";
			sales.open("supermarket_sale_sets.txt", ios::in);

			string a, b, c;
			cout<<"\t\t\t\t Enter 3 codes that will be included in a sale set.\n";
			cout<<"\t\t\t\t Code 1: ";	cin>>a;
			cout<<"\t\t\t\t Code 2: ";	cin>>b;
			cout<<"\t\t\t\t Code 3: ";	cin>>c;
			vector<string> newSet = {a, b, c};

			if (!sales){
				sales.open("supermarket_sale_sets.txt", ios::app|ios::out);
				sales<<a<<b<<c;
				cout<<"\t\t\t\t Successfully added!\n";
			} else {
				bool present;

				string x, y, z;
				sales>>x>>y>>z;
				while(!sales.eof()){
					if(find(newSet.begin(), newSet.end(), x)!=newSet.end() 
					&& find(newSet.begin(), newSet.end(), y)!=newSet.end() 
					&& find(newSet.begin(), newSet.end(), z)!=newSet.end()){ //a line in the file matches the codes input (order does not matter)
						present = true;
						cout<<"This set was already on sale.\n";
						break;
					}
					present=false;
				}
				if (!present){//sale doesn't exist yet (in this specific order)
					//has to close and re-open file with new permissions
					sales.close();
					sales.open("supermarket_sale_sets.txt", ios::app|ios::out);
					sales<<a<<b<<c;
					cout<<"\t\t\t\t Successfully added!\n";
				}
			}

		}

		//admin menu option that removes a line from "supermarket_sale_sets.txt"
		void removeSet(){
			cout<<"\t\t\t Remove Sale Set\n";
			sales.open("supermarket_sale_sets.txt", ios::in);
			if (!sales){
				cout<<"Failed to open sales sets!\n";
				return;
			}
			fstream data;
			data.open("temp.txt", ios::out);
			
			string a, b, c;
			cout<<"\t\t\t\t Enter all the codes in the sale to be removed (order does not matter): \n";
			cout<<"\t\t\t\t Code 1: ";	cin>>a;
			cout<<"\t\t\t\t Code 2: ";	cin>>b;
			cout<<"\t\t\t\t Code 3: ";	cin>>c;
			vector<string> saleVec = {a, b, c};
			cout<<"Entered: "<<a<<", "<<b<<", "<<c<<"\n";

			string x, y, z;
			bool present = false;
			while(sales>>x>>y>>z){ //searches sale set file line by line
				if(find(saleVec.begin(), saleVec.end(), x)!=saleVec.end() 
					&& find(saleVec.begin(), saleVec.end(), y)!=saleVec.end() 
					&& find(saleVec.begin(), saleVec.end(), z)!=saleVec.end())
				{
					//if all three codes of the set are present in the vector of codes, it is not copied into the new file
					present = true;
				} else {
					data<<x<<" "<<y<<" "<<z<<"\n";
				}
			}

			sales.close();
			data.close();

			if(present){
				//delete old file, and rename the new one (missing the sale set to be removed)
				remove("supermarket_sale_sets.txt");
				rename("temp.txt", "supermarket_sale_sets.txt");
				cout<<"Removed!\n";
			} else {
				remove("temp.txt");
				cout<<"Could not find that sale set.\n";

			}
			
			return;
		}

		//prints customer receipt
		void receipt(){
			float total = 0;
			float amount;
			int quantity;

			if(sizeof(basketCodeArr)==0){ //no items were "scanned" by the customer
				cout<<"\n__________________________________________________________";
				cout<<"\n| \t\t\t\t\t Your bag is empty! \t\t\t\t\t |";
				cout<<"\n__________________________________________________________";
			} else {
				inventory.open("supermarket_inventory.txt", ios::in);
				if (!inventory){ //if cannot open file, quit
					cout<<"\nWe apologize for the inconvenience, we cannot access our inventory at this time!\n";
					return;
				}
				inventory.close();

				//Print Receipt header
				cout<<"\n\n\n"<<setw(64)<<setfill('_');
				cout << "\n|"<< setw(35) << setfill(' ')<< "RECEIPT"<< setw(27) << setfill(' ')<< "|";
				cout << "\n|"<<setw(10)<<"Item No."
							<<setw(20)<<"Item Name"
							<<setw(11)<<"Quantity" 
							<<setw(9)<<"Price"
							<<setw(11)<<"Amount"<<"|";


				map<string, int>::iterator it = basketCodeArr.begin();
				vector<string> unknownCodes;
				
				while (it !=basketCodeArr.end()) {  //parse each item in the basket
					inventory.open("supermarket_inventory.txt", ios::in);
					inventory>>productCode>>productName>>productPrice;

					while(!inventory.eof()){ //parse every product in the inventory
						if(productCode == it->first){ //the code in the basket exists in inventory
							quantity = it->second;

							//check for three of a kind							
							if (quantity >= 3){
								threeForTwo(productCode, quantity, productPrice);
								int groups = quantity/3;
								int remainder = quantity%3;
								amount = (productPrice*groups*2) + (productPrice * remainder);

							} else {
								amount = productPrice * quantity;
							}

							//print the item line to the receipt
							cout << "\n|"
								<< setw(10) << productCode
								<< setw(20) << productName
								<< setw(11) << quantity
								<< setw(9) << fixed << setprecision(2) << productPrice
								<< setw(11) << fixed << setprecision(2) << amount
								<< "|";

							if (quantity >= 3){ //if three identical items (discount was applied)
								//display a note the the customer saved due to the discount
								cout<<"\n|"<<setw(46)<<discounts.begin()->first
									<< fixed << setprecision(2) << discounts.begin()->second
									<<setw(12)<<"|";
							}

							total += amount;
							break;
						}
						inventory>>productCode>>productName>>productPrice;

					}
					inventory.close();
					++it;
				}

				for (map<string, int>::iterator it = basketCodeArr.begin(); it != basketCodeArr.end();) {
					if (it->second == 0){ //delete anything from the basket that has already been discounted
						it = basketCodeArr.erase(it);
					} else { it++; }
				}

				float discount = setOfThree(); //check for discounts available from the sets
				total-=discount;

				//print footer of the receipt
				cout<<"\n|"<<setw(62)<<setfill('_')<<"|";
				cout<<"\n"<<setw(58)<<setfill(' ')<<"Total Amount: $"
						<< fixed << setprecision(2)<<total<<"\n";
				inventory.close();
			}
			basketCodeArr.clear();
			return;
		}

		//calculates discount for purchasing three identical items, available for multiples of three, where user pays for 2
		void threeForTwo(string code, int quantity, float itemPrice){
			int groups = quantity/3;
			int remainder = quantity%3;
			basketCodeArr[code]=remainder;

			float discount = groups * itemPrice;
			string discountMessage = "Buy 2 get 1 free! You saved $";

			discounts[discountMessage] = discount;
		}

		//calculates discount for buy a set of three items, the cheapest is free
		float setOfThree(){
			if (!sales.is_open()){
				sales.open("supermarket_sale_sets.txt", ios::in);
			}

			string x, y, z;
			float amountSaved = 0;

			while (sales>>x>>y>>z){
				//if codes in the sales file are present in the "basket"
				if (basketCodeArr.find(x)!=basketCodeArr.end() 
					&& basketCodeArr.find(y)!=basketCodeArr.end() 
					&& basketCodeArr.find(z)!=basketCodeArr.end())
				{
					//find the cheapest of the set
					float lowestPrice = minOfThree(x, y, z);
					amountSaved+=lowestPrice;
					
					//print that the sale was applied
					cout<<"\n|"<<setw(26)<<setfill(' ')<<"You bundled and saved! "
						<<x<<", "<<y<<", "<<z
						<<" - $"<<fixed<<setprecision(2)<<lowestPrice
						<<setw(12)<<setfill(' ')<<"|";

					//erase all codes from "basket" so they cannot be used for another discount
					basketCodeArr.erase(x);
					basketCodeArr.erase(y);
					basketCodeArr.erase(z);
					break;
				}

			}

			sales.close();
			return amountSaved;

		}

		//returns the lowest price of the three products provided
		float minOfThree(string& x, string& y, string& z){
			if (!inventory.is_open()){
				inventory.open("supermarket_inventory.txt", ios::in);
			}

			map<string, float> discountSet;
			string c, n;
			float p;

			while(inventory>>c>>n>>p){
				//assign cost to the corresponding code in the map
				if (c==x||c==y||c==x){ discountSet[c] = p; } 
			}

			inventory.close();
 
			//return the cheapest price in the map
			float minPrice = numeric_limits<float>::max();
			if (discountSet.find(x) != discountSet.end() && discountSet[x] < minPrice) {
				minPrice = discountSet[x];
			}
			if (discountSet.find(y) != discountSet.end() && discountSet[y] < minPrice) {
				minPrice = discountSet[y];
			}
			if (discountSet.find(z) != discountSet.end() && discountSet[z] < minPrice) {
				minPrice = discountSet[z];
			}

			return minPrice;

		}
};

int main() {
	cout<<"\n\n\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
	cout<<"\t\t Welcome to The Mini Supermarket by Jillian Desmond!";
	cout<<"\t\t\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n\n";

	supermarket s;
	s.menu();

	return 0;
}