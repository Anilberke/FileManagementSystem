#!/bin/awk -f

# Sample AWK script for updating inventory file

BEGIN {
    FS = OFS = " ";  # Set field separator as space
}

# Function to update existing entry or add a new entry
function update_entry(name, price, quantity) {
    found = 0;
    
    # Iterate through the inventory
    for (i = 1; i <= length(inventory); i++) {
        if (inventory[i]["name"] == name) {
            # Update existing entry
            inventory[i]["price"] = price;
            inventory[i]["quantity"] = quantity;
            found = 1;
            break;
        }
    }

    if (!found) {
        # Add a new entry
        inventory[length(inventory) + 1]["name"] = name;
        inventory[length(inventory)]["price"] = price;
        inventory[length(inventory)]["quantity"] = quantity;
    }
}

# Read the input file
{
    name = $1;
    price = $2;
    quantity = $3;

    # Update or add the entry
    update_entry(name, price, quantity);
}

# Print the updated inventory
END {
    for (i = 1; i <= length(inventory); i++) {
        print inventory[i]["name"], inventory[i]["price"], inventory[i]["quantity"];
    }
}
