var value_b17_config_x = [];
value_b17_config_x[0] = 'x';
var site_value = [];
site_value[0] = value_b17_config_x; 




function myC3_1()
{
	var pia = [];


	 band_list.forEach(function(element) {    	
    	//console.log(element);
    	pia.push(element);
    	pia.push(band_marker.get(element).length);
    });

	createChart(pia);
	

    var chart3 = c3.generate({
           bindto: '#chart3',
    data: {
      x:'x',
        columns: site_value,
        types: {
            data1: 'spline'
           
        },
        
	    }
	});

	G_chart();
}



function createChart(pia_data)
{
    var column = [];
    for(var i=0; i< pia_data.length; i+=2)
    {
        column[i/2] = [ pia_data[i],pia_data[i+1]];
    }

	var chart4 = c3.generate({
           bindto: '#chart4',
		   data: {
		        columns: column,
		        types: {
		            data1: 'area',
		            data2: 'area-spline'
		        }
		    }
	});
        chart4.transform('pie');
}

function G_chart() 
{
	
      google.charts.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['ID', 'X', 'Y', 'Temperature'],
          ['',   80,  167,      12],
          ['',   79,  136,      13],
          ['',   78,  184,      5],
          ['',   72,  278,      23],
          ['',   81,  200,      21],
          ['',   72,  170,      10],
          ['',   68,  477,      8]
        ]);

        var options = {
          colorAxis: {colors: ['yellow', 'red']}
        };

        var chart = new google.visualization.BubbleChart(document.getElementById('chart_div'));
        chart.draw(data, options);
      }
}