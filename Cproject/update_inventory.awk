# update_inventory.awk

# Function to check if an entry exists in the file
function entryExists(name, arr) {
    return (name in arr);
}

# Function to update an entry
function updateEntry(name, price, quantity, arr) {
    arr[name] = sprintf("%s %d %d", name, price, quantity);
}

# Function to delete an entry
function deleteEntry(name, arr) {
    delete arr[name];
}

# Function to print the inventory array
function printInventory(arr) {
    for (entry in arr) {
        print arr[entry];
    }
}

# Read the operation and entry details from the command line
BEGIN {
    operation = ARGV[1];
    entryName = ARGV[2];
    entryPrice = ARGV[3] + 0;  # Convert to numeric
    entryQuantity = ARGV[4] + 0;  # Convert to numeric

    # Remove the command line arguments
    ARGV[1] = ARGV[2] = ARGV[3] = ARGV[4] = ""
}

# Read the file and perform the update
{
    inventory[$1] = $0;
}

# Perform the update based on the operation
END {
    if (operation == "u" && entryExists(entryName, inventory)) {
        # Update entry
        updateEntry(entryName, entryPrice, entryQuantity, inventory);
    } else if (operation == "a") {
        # Add entry
        inventory[entryName] = sprintf("%s %d %d", entryName, entryPrice, entryQuantity);
    } else if (operation == "d" && entryExists(entryName, inventory)) {
        # Delete entry
        deleteEntry(entryName, inventory);
    }

    # Print the updated inventory
    printInventory(inventory);
}

