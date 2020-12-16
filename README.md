![alt text](https://github.com/aroyer-qc/SkinGUI_Builder/blob/master/Ressource/Digini.png)

# SkinGUI_Builder for Digini

Create skin file to use in project using touch screen for embedded system

It is a work in progress to provide complete skin data and configuration for Digini GUI system for embedded project.

Configuration for display size include some standard size, and custom configuration support.
Application can convert format RGB565 <->ARGB8888
Can generate source file from image and provide information for the application for the image.

So Far. this project build a file xxx.skn
  it allow to encapsulated in the skin file
    - image PNG, JPG, GIF.
    - Font of any size.

All widget and page configuration is handle in the embedded system at this time. 

It also compress the data with different method to limit loading time in system via file.
Reach very often ratio of 4:1 

Note(s)

TODO in this application
    
    - Sound file
    - Menu page configuration
      - Widget
      - Label and label list
      - Service
      
![alt text](https://github.com/aroyer-qc/SkinGUI_Builder/blob/master/chart.png)
