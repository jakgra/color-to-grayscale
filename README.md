## Replace hex color codes with their grayscale counterparts in all text files
### Disclaimer
Attention. Use at your own risk, make a backup of all the files passed to the program.
If you run out of RAM, some files will be lost otherwise.  
### Installation
To build:
```bash
git clone https://github.com/jakgra/color-to-grayscale
cd color-to-grayscale
git clone https://github.com/jakgra/toolbox
git clone https://github.com/websnarf/bstrlib
./build.sh
```
To install:
```bash
sudo cp color_to_grayscale /somewhere/on/your/PATH/
```
### Usage
For example, to replace the color codes for all css and svg files in the current directory run this in a terminal.
```bash
color_to_grayscale *.css *.svg
```
