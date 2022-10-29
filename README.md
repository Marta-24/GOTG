<h1>
    <img style="vertical-align:middle" src="Editor/resources/icons/wiwa_icon.png" width="30">
    Wiwa Engine
</h1>
<img src="images/editor.png">
<h2>About the project</h2>
Wiwa engine is a 3D Game Engine developed during the Game Engines subject. Our goal is to fulfill an Engine that can be used to develop all kinds of games. The engine is based on a full ECS system, and it's meant to have modern features as PBR and C# scripting.
<h2>Info</h2>
For now the engine has the following features.

- Reflection
- Create entites
- Add components
- Asign materials
- WIP material system
- WIP basic lighting

<h2>Authors</h2>
Currently we are two students working on the project

- Víctor Falcón <a href="https://github.com/ZaroDev">@ZaroDev</a>
- Pablo Llorente <a href="https://github.com/Xymaru">@Xymaru</a>

<h2>Installing</h2>

Dependencies:

- Visual Studio 2019 or more

In order to install the project you can

```
git clone https://github.com/ZaroDev/WiwaEngine
```

Open the ``Scripts`` folder and then select your Visual Studio version at ``GenerateProjectsX.bat``

Advise: the project only supports Windows
<h2>How to use</h2>
<h3>Camera:</h3>

- Hold right click to look  
- Right click + WASD to move
- F to focus the selected entity
- Alt + right click to orbit an entity
- Alt + Q to quit the app
- Q to unselect the current gizmo
- W to move an object
- E to rotate an object (WIP)
- R to scale

<h3>Materials</h3>
To create a material click the button Create Material at the Assets window.
<img src="images/creatematerial.png">

Assing a name to the material and click create.

<img src="images/creatematerialwindow.png">

Open the material editor window and select a material you want to edit.
<img src="images/materialwindow.png">

<h2>MIT License</h2>
Copyright (c) 2022 Xymaru & ZaroDev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
