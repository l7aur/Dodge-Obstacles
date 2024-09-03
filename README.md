# Dodge Obstacles v. 1.0

## Table of contents

- [Preview](#preview)
- [Description](#description)
- [Player Controls](#player-controls)
- [How To Play](#how-to-play)
- [Resources](#resources)
- [Further-Developments](#further-developments)

### Preview

*video*

### Description

**Dodge Obstacles** is a 2D game that works inside the Linux file explorer. The file explorer window where the source files are should be resized so that 15 files can be displayed on a row and 7 such rows are visible. Once the source file is compiled and run one must select the terminal and watch what happens in the file explorer window. 

15*7=105 .bmp files are generated:
- 104 are blank files (*block.bmp* - white image)
- 1 file (*player.bmp* - green file) - the file that the player can control

*Note:* At random times, *obstacle.bmp* (black iamges) files are created in the rightmost column and travel to the left. When they reach the first column such files are destroyed and replaced by *block.bmp* files.

### Player Controls

- **W** the *player.bmp* moves up by one row
- **S** the *player.bmp* moves down by one row

### How To Play

The player has to control its *player.bmp* file so that it does not crash into *obstacle.bmp* files.

### Resources

### Further Developments

While working at this project I discovered there is a huge bottleneck created by syscall usage. Each time a bitmap file is generated (i.e. created byte by byte) a noticeble and increasingly annoying pause appears. I see no point in further developing this idea because the delay created by the .bmp file generation massively affects the player experience. A possible solution would be to change the file format to something that is computationly less demanding. However, using simple Linux (byte) files provides no visual representation and makes the player experience tedious: one has to check the names of the files each time something changes making it difficult to spot obstacles and dodge them. A visual GUI provides great help with this matter, but seems too much for syscalls to handle.  
