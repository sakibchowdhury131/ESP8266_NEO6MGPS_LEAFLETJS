# import gmplot package
import gmplot
#Set different latitude and longitude points
Charminar_top_attraction_lats, Charminar_top_attraction_lons = zip(*[
   (17.3833, 78.4011),(17.4239, 78.4738),(17.3713, 78.4804),(17.3616, 78.4747),
   (17.3578, 78.4717),(17.3604, 78.4736),(17.2543, 78.6808),(17.4062, 78.4691),
   (17.3950, 78.3968),(17.3587, 78.2988),(17.4156, 78.4750)])



#declare the center of the map, and how much we want the map zoomed in
gmap3 = gmplot.GoogleMapPlotter(17.3616, 78.4747, 13)
# Scatter map
gmap3.scatter( Charminar_top_attraction_lats, Charminar_top_attraction_lons, '#FF0000',size = 50, marker = False )
# Plot method Draw a line in between given coordinates
gmap3.plot(Charminar_top_attraction_lats, Charminar_top_attraction_lons, 'cornflowerblue', edge_width = 3.0)
#Your Google_API_Key
gmap3.apikey = 'AIzaSyAIB5L-3V1u0iA-11Sk7PafVv6JgV11Pkg'
# save it to html
gmap3.draw(r'map.html')