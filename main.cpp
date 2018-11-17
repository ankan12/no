
#include <iostream>
#include <fstream>
#include "SDL_Plotter.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Wave.h"
#include "Number.h"
#include "Video.h"
#include "Level.h"
#include "DrawingFunctions.h"
#include "WaveAnimation.h"
#include <cstdlib>
using namespace std;


int main(int argc, char ** argv)
{
    cout << "Hello!" << endl;

    const int screenWidth = 800, screenHeight = 400;
    SDL_Plotter p(screenHeight, screenWidth);

    ifstream inFile;

    Sprite spawnPipeLeft("animatedPipe.txt", inFile);
    spawnPipeLeft.set_y(50);
    spawnPipeLeft.setScale(2);

    Sprite spawnPipeRight("animatedPipe.txt", inFile);
    spawnPipeRight.set_y(50);
    spawnPipeRight.setMirrored(true);
    spawnPipeRight.setScale(2);
    spawnPipeRight.set_x(screenWidth - spawnPipeRight.getWidth(0) *
                         spawnPipeRight.getScale());

    Sprite brickFloor("brickFloor.txt", inFile);
    brickFloor.setScale(2);
    brickFloor.set_y(screenHeight - brickFloor.getScaledHeight(0));

    Level level1;
    level1.setBlockSprite("blueBlock.txt", inFile, 2);

    level1.placePlatform(0, spawnPipeLeft.get_y() +
                         spawnPipeLeft.getScaledHeight(0), 25);

    level1.placePlatform(800 - 25 * 14, level1.getPlatform(0).get_y(), 25);

    cout << (200 -level1.getPlatform(0).get_y()) << endl;

    level1.placePlatform(400 - 15 * 14, 200, 30);

    level1.placePlatform(0, 290, 20);
    level1.placePlatform(800 - 20 * 14, 290, 20);

    level1.placePlatform(0, 210, 10);
    level1.placePlatform(800 - 10 * 14, 210, 10);

    Sprite bottomLeftPipe("bottomPipe.txt", inFile);
    Sprite bottomRightPipe("bottomPipe.txt", inFile);

    bottomLeftPipe.setScale(2);
    bottomRightPipe.setScale(2);

    bottomLeftPipe.setMirrored(true);

    bottomLeftPipe.set_y(400 - brickFloor.getScaledHeight(0) -
                         bottomLeftPipe.getScaledHeight(0));
    bottomRightPipe.setLocation(800 - bottomRightPipe.getScaledWidth(0),
                                bottomLeftPipe.get_y());

    WaveAnimation wa(0, 50);
    wa.setSpeed_1sthalf(0.2);
    wa.setSpeed_2ndhalf(0.2);

    vector<WaveAnimation> wAnimations;

    while (!p.getQuit()){

        if (p.kbhit()){
            switch(p.getKey()){
            case ' ':

                int p = rand() % level1.numOfPlatforms();

                int col = rand() % level1.getPlatform(p).getColLocations().size();

                WaveAnimation wa(p, col);

                wAnimations.push_back(wa);

            }
        }

        p.clear();

        drawBlackBackground(screenWidth, screenHeight, p);

        spawnPipeLeft.draw(p);
        spawnPipeRight.draw(p);

        bottomLeftPipe.draw(p);
        bottomRightPipe.draw(p);

        horizontalTile(brickFloor, 0, screenWidth, p);

        wa.setNextFrame(level1.getPlatform(0).getColLocations());

        for (int w = 0; w < wAnimations.size(); w++){

            WaveAnimation& wa = wAnimations[w];

            if (wa.finished()){
                wAnimations.erase(wAnimations.begin() + w);
                continue;
            }

            Platform& p = level1.getPlatform(wa.platformNum());

            wa.setNextFrame(p.getColLocations());

        }

        level1.draw(p);

        p.update();

        p.Sleep(1);

    }

    return 0;
}
