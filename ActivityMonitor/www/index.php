<?php
    while ($entry = fscanf($handle, "%d:%d:%d, %s %s %s %s %s\n"))
    {
        if( $entry[7] == "true" )
        {
            $hour_motion[ $entry[0] ]+= 1;
        }
    }
    echo "<html><head></head><body><br>\n";

    foreach (glob("./logs/*.txt") as $filename) {
        $keywords = preg_split("/[\s\/.\"]+/", $filename);
        echo "<a href=bar.php?filename=" . $filename .
             "> <font size=\"6\">" .
             $keywords[2] . "-" .
             $keywords[3] . "-" .
             $keywords[4] . "</font></a><br>\n";
        //print_r($keywords);
    }

    printf("</body></html>");
?>