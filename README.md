# COMP2811 Coursework 3 - Final Project

## Water Quality Monitor
This App displays data about the water quality in England.

There are 5 main pages:
- the dashboard
- pollutant overview
- environmental litter
- flourinated compounds
- compliance dashboard

Each page shows graphs or tables to make the data easily understanable and give an overview.

There is also a table view which was mainly made for the first developing stage to see how the data can be accessed though now it can be used if one is looking for a specific measurement or wants to see the raw data.

## How to run the application

Set up the build process with

    mkdir build && cd build && cmake ..

Build the application with

    make 

Run the application with

    ./window

Make sure you have the Y-2024.csv file containing the data in the coursework directory

## Limitations

We decided aggainst implementing the extra geographic hotspots page as it was optional. We also only have 4 pages implemented as one of our group members was sick for quite some time and didn´t have time for that.

Of the language choices only German contains some actual translations for the Main Window, the Dashboard and the flourinated Compounds page. The other languages .ts files are created and just need the strings to be translated, same goes for the remaining German strings on other pages.

The links on the dashboard (from the dashboard to each of the other pages) take some time to load at times without feedback. Some of the charts on the Dashboard don´t contain the real data.

On the flourinated compounds page the compliance is only a dummy feature which does not respect the actual data. The rollover pop-ups don´t provide much information yet.

On the compliance dashboard the filters are only dummy with no implemented backend.

Accessibility is limited. 
- We tried to make colours as accessible as possible for visiually impaired and colour blind by adding high contrasts and choosing colours like and red and green with different values. However, none of us are actually affected or no anyone that is so we couldn´t if this actually is true.
- The tabs can not only be accessed by clicking on them but also using keyboard shortcuts.
- We used the standard Qt widgets and tried applying meaningful names as to make Screen Readers possible.
- We tried to make keyboard navigation for all interfaces besides the Tab bar as well. We might have missed some of those though as we did not have this in the first two iterations. 
