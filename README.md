# Supermarket
A simple command line system for supermarket checkout lanes.


Terminal Commands to run the program:
-------------------------------------
Mac:
> g++ -o supermarket supermarket_Mac.cpp

> ./supermarket

Windows:
> g++ -o supermarket supermarket_Win.cpp

> ./supermarket

Login credentials for Admin role:
---------------------------------
email: "admin@gmail.com"

password: HireMe!

Flow chart:

    -> Main Menu
        -> choose: admin -> Admin Menu 
            -> choose: add inventory -> compete -> LOOP to Admin Menu
            -> choose: remove inventory -> compete -> LOOP to Admin Menu
            -> choose: define sale set  -> compete -> LOOP to Admin Menu
            -> choose: remove sale set -> compete -> LOOP to Admin Menu
            -> choose: exit -> RETURN to Main Menu
        -> choose: customer -> "scan" -> reciept -> LOOP to Main Menu
        -> choose: exit -> program terminates


### Notes:
- Items can only be apart of one discount. Discounted items are removed from the "basket". 
- Undiscounted items will remain in the "basket" and therefore remain eligible for dicount.
- User adds items to their "basket" by inputting the product code.
- System stores items in "basket" as Key Value pairs, item code and the quantity.
- Admin has the ability to add products to inventory. Product codes, names, and prices are stored in a text file.
- Admin has the ability to remove products from inventory by searching product code.
- Admin has the ability to define sale sets as groups of three product codes. Sets are saved in a text file.
- Admin has the ability to remove sale sets by inputting the three codes in the set.

  When you buy 3 of the same item, get them for the price of 2:
  - The system finds the item code from the "basket" in the inventory file, and retrieves the associated name and price
  - If the quantity in the basket is greater than or equal to 3, the system discounts the amount for all multiples of three
  - Any remaining items (not in a group of three), will be full price

  Buy 3 different items in a set, get the cheapest one free:
  - The system checks for a sale set after the three of a kind discount has been applied.
  - The system evaluates for a sale set after all item lines are printed.
  - The system checks for sets by parsing the sale set text file line-by-line, and searching for all three product codes in the "basket".
