# Design Ideas

*  content part
    *   推荐系统
*  iOS part
    *   完整内容
    *   up vote / down vote
*  Pebble part
    *   看title/abstract，可以继续在手机上看完整的
   *  novelty
      *   Use Pebbles features
          *   sensors
      *   Art and Design
          *   1 bit black & white: retro, dither art
      *   Social Aspect: interaction

# [Pebble Guide](http://developer.getpebble.com/guides/)

*   App Events
    *   Managing App Interruptions
    *   Measuring Battery Level
    *   Managing Bluetooth Events
    *   Using The Current Time
    *   Using the Wakeup API
*   Display & Animations
    *   Drawing Graphics
    *   UI Layers
    *   Animating Layers
    *   App Resources (Fonts & Images)
    *   Using Pebble Fonts
*   Communications
    *   App Communication
    *   Synchronizing App UI
    *   Data Logging on Pebble
*   Sensors
    *   Detecting Acceleration
    *   Determining Direction
*   App Structure
    *   App Metadata (Native SDK only)
    *   Implementing the Background Worker
    *   Button Clicks
    *   Persisting App Data
    *   App Windows

# [Pebble Documentation](http://developer.getpebble.com/docs/)

*   PEBBLE C
*   PEBBLEKIT ANDROID
*   PEBBLEKIT IOS
*   PEBBLEKIT JAVASCRIPT
*   PEBBLE.JS

# [Pebble Tutorial](http://developer.getpebble.com/getting-started/)

### WATCHFACE TUTORIAL

*   Build Your Own Watchface
    1. Created a new CloudPebble project.
    1. Setup basic app structure.
    1. Setup a main Window.
    1. Setup a TextLayer to display the time.
    1. Subscribed to TickTimerService to get updates on the time, and wrote these to a buffer for display in the TextLayer
*   Customize Your Watchface
    1. Resource management
    1. Custom fonts (using GFont)
    1. Images (using GBitmap and BitmapLayer)
*   Adding Content From The Web
    1. Managing multiple font sizes.
    1. Preparing and opening AppMessage.
    1. Setting up PebbleKit JS for interaction with the web.
    1. Getting the user's current location with navigator.getCurrentPosition().
    1. Extracting information from a JSON response.
    1. Sending AppMessage to and from the watch.
        1. Create AppMessage callback functions to process incoming messages and errors.
        1. Register this callback with the system.
        1. Open AppMessage to allow app communication.

### PEBBLE.JS TUTORIAL

*   Build A Watchapp with Pebble.js
*   Expanded Weather Forecasts
*   Adding More Details
    1. Built entirely in JavaScript
    1. Hourly forecast in a location of your choice from the web
    1. Sub-Menu Windows containing hour-specific information including temperature, windspeed and pressure.
    1. 'Shake-to-update' functionality, including a vibration upon update completion.