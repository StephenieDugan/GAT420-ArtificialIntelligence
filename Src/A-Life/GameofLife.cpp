#include "GameofLife.h"
#include "Texture.h"
#include "Random.h"
#include <fstream>
#include <cstring>

bool GameofLife::Initialize()
{
	// Resize both buffer A and B to the number of cells
	bufferA.resize(size.x * size.y);
	bufferB.resize(size.x * size.y);
	return true;
}

void GameofLife::Step()
{
    frame++;

    std::vector<uint8_t>& readBuffer = (frame % 2 == 0) ? bufferA : bufferB;
    std::vector<uint8_t>& writeBuffer = (frame % 2 == 0) ? bufferB : bufferA;

    // Update buffer
    std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            int weight = 0;

            // Read surrounding 8 cells, accumulate weight
            // XXX
            // X0X
            // XXX
            weight += 
                Read<uint8_t>(readBuffer, x - 1, y - 1) + Read<uint8_t>(readBuffer, x, y - 1) + Read<uint8_t>(readBuffer, x + 1, y - 1) +
                Read<uint8_t>(readBuffer, x - 1, y) +      /* Middle cell taking weight*/       Read<uint8_t>(readBuffer, x + 1, y) +
                Read<uint8_t>(readBuffer, x - 1, y + 1) + Read<uint8_t>(readBuffer, x, y + 1) + Read<uint8_t>(readBuffer, x + 1, y + 1);

            // Game of Life rules
            if (Read<uint8_t>(readBuffer, x, y))
            {
                if (weight < 2 || weight > 3)
                {
                    Write<uint8_t>(writeBuffer, x, y, 0);
                }
                else
                {  Write<uint8_t>(writeBuffer, x, y, 1);
                }
                   
            }
            else
            {
                if (weight == 3)
                    Write<uint8_t>(writeBuffer, x, y, 1);
            }
        }
    }

    std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v)
        {
            { return  v ? 0xffffffff : 0; }
        });

    // copy color buffer to texture
    texture->Copy(color_buffer);
}

void GameofLife::KeyDown(SDL_Keycode keycode)
{
    if (keycode == SDLK_s)
    { 
        // write random 'alive' cells to buffer
        std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;
        for (int i = 0; i < writeBuffer.size(); i++) writeBuffer[i] = (random(0, 10) == 0) ? 1 : 0;
    }
}


