# delete.awk

# Set the field separator to whitespace
BEGIN {
    FS = " "
}

# Check if the first field (item name) matches the specified name to delete
$1 != itemName {
    # If not, print the entire line
    print $0
}

# End of script



