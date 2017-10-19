<?php
    echo "Hello " . $_GET["name"] . ",";
    echo " your VCC is: " . $_GET["VCC"] . "";
    echo " motion is: " . $_GET["motion"] . "";

    echo "<br>";

    $today = getdate();

    echo "<br>";
    echo "Today is " . date("Y/m/d") . "<br>";

    $logfile = fopen("motion.txt", "a");
    $txt = "motion = ";
    fwrite($myfile, $txt);
    $txt = $_GET["motion"];
    fwrite($myfile, $txt);
    fwrite($myfile, "\n");
    fclose($myfile);

    if( $_GET["name"] == "ESP" )
    {
        $filename = "logs/" . date("Y.m.d") . ".txt";
        $logfile = fopen($filename, "a");
        $text = date("H:i:s") . ", ";
        fwrite($logfile, $text);
        $text = $_GET["name"] . ", VCC: " . $_GET["VCC"] . ", motion: " . $_GET["motion"] . "\n";
        fwrite($logfile, $text );
        fclose($logfile);
    }
?>
