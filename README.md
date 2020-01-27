# MLBDemo
MLB Scores demo

# Useage
left and right arrows move the tiles.

up and down arrows adjust the date.

enter will pop up a details screen.  enter again will close it.

# Viewing code
This was built on a macbook pro using Cocos2D.  So to open the code, go into the proj.ios_mac folder and open the MLB xcodeproj file.

It makes use of curl and includes rapidjson for the json parsing.  If it fails to build because of curl, you can install it via homebrew, brew install curl\
if the linking fails, you might need to add the libcurl.4.dylib file to the frameworks in xcode.  This will be found where homebrew installs curl.  Rapidjson is included so should just build.

if you want to build on Windows, there is a solution and project file in the proj_win32 folder.  I think it would build but you likely need to add the additional files.

if you need to install Cocos2d, I built it using version 3.17 which can be found at https://www.cocos.com/en/cocos2dx

# Extra Credit
1. I would have done this but, it isn't baseball season currently.  You can find an example of setting the current date in MLBScene.cpp at line 90.  If you uncomment then it sets it to the current date but there are no winter games going on.
2. you can change the dates using the up and down arrows.  It's throttled so if you bang on the arrows it won't move until the query completes.
3. I did not do this part
4. I made a simple screen that displays some information.  It's nothing fancy though

# Known Issues
1. I took the logos from the MLB site.  They were in svg format which I then converted to png.  Unfortunately, there are some strange artificats on some when displaying that I don't see in an image viewer.
Also, the San Diego padres logo is brownish and doesn't display well.
2. Screen is optimized for the 1900 x 1080 of the background image
3. loading of json can be a bit slow as you switch days.


