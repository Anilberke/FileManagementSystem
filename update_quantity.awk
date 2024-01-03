BEGIN {
    FS = OFS = " ";
}

{
    if ($1 == itemName) {
        $3 = newQuantity;
    }
    print $0;
}
