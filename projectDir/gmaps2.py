import gmaps
import gmaps.datasets
import pandas as pd


gmaps.configure(api_key = 'AIzaSyAIB5L-3V1u0iA-11Sk7PafVv6JgV11Pkg')
earthquake_df = gmaps.datasets.load_dataset_as_df('earthquakes')
print(earthquake_df.head())


locations = earthquake_df[['latitude', 'longitude']]

weights = pd.to_numeric(earthquake_df['magnitude'], downcast="float")

fig = gmaps.figure()
x = gmaps.heatmap_layer(locations, weights = weights)
fig.add_layer(x)
fig

