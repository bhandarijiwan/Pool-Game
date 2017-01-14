#  Simple Pool Game

Opengl project implementing Pool game.

# Install SOIL

 ```bash
 sudo apt-get install libsoil-dev
 ```


# Install Bullet
 ```bash
 sudo apt-get install libbullet-dev
 ```

## Building and Running

This project has a dependency on SOIL library. Install SOIL first following the above instructions.

To build this project follow the steps below.

Running the make in a separate directory will allow easy cleanup of the build data, and an easy way to prevent unnecessary data to be added to the git repository.  

```bash
mkdir build
cd build
cmake ..
make
```

### Running Instructions.
```./PoolGame ``` Will start the Game.
 
### Keyboard and Mouse Interaction
<ul>
	<li> Left bracket key --> Press, hold, and release left bracket button. Shoots the cue ball. Change the intensity of the power of your shot by holding the key down longer (up to a limit. No unlimited power for you!</li>
    <li> R Key --> Reset the game manually.</li>    
    <li> Number Pad 4 Key --> Rotate the cue stick around the cue ball to the left.</li>  
    <li> Number Pad 6 Key --> Rotate the cue stick around the cue ball to the right.</li>        
    <li> Up/Down Arrrow --> Zoom In /Zoom Out ( Translate the camera forward/backward) (Zoom out to be able to see pluto.)  </li>
    <li> Q/Z Key --> Increase/Decrease ambient brightness.</li>
    <li> Left mouse --> panning.</li>
    <li> E Key --> Increase spotlight radius.</li>	
    <li> C Key --> Decrease spotlight radius.</li>	
</ul>
