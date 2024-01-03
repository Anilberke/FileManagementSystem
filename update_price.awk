# update_price.awk
# Usage: awk -v itemName=<item_name> -v newPrice=<new_price> -f update_price.awk details.txt > temp.txt && mv temp.txt details.txt

BEGIN {
    FS = OFS = " ";
}

{
    
    if ($1 == itemName) {
        
        $2 = newPrice;
    }

   
    print $0;
}

