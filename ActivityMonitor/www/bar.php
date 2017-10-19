<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['bar']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        var data = google.visualization.arrayToDataTable([
      <?php
         $handle = fopen($_GET["filename"], "r");
         while ($entry = fscanf($handle, "%d:%d:%d, %s %s %s %s %s\n"))
         {
             if( $entry[7] == "true" )
             {
                  $hour_motion[ $entry[0] ]+= 1;
             }
         }

         $keywords = preg_split("/[\s\/.]+/", $_GET["filename"]);

         printf( "\t\t['%d-%d-%d', 'minutes'],\n", $keywords[2], $keywords[3], $keywords[4]);

         for($i = 0; $i < 23; $i++)
         {
             printf("\t\t\t['%d', %d],\n", $i, $hour_motion[$i] > 60 ? 60 : $hour_motion[$i]);
         }
         printf("\t\t\t['%d', %d] ]);\n", 23, $hour_motion[23]);
      ?>
        var options = {
          chart: {
            title: 'Gabinet utilization',
      <?php
         $keywords = preg_split("/[\s\/.]+/", $_GET["filename"]);
         printf("\t\tsubtitle: 'Minutes of activity on %d-%d-%d',",$keywords[2], $keywords[3], $keywords[4]);
      ?>
          }
        };

        var chart = new google.charts.Bar(document.getElementById('columnchart_material'));

        chart.draw(data, google.charts.Bar.convertOptions(options));
      }
    </script>
  </head>
  <body>
    <div id="columnchart_material" style="width: 800px; height: 500px;"></div>
  </body>
</html>
