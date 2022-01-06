#pragma once
#include <fstream>
#include <iostream>
#include "para.h"
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>

void writeData(std::string configPath);
bool readData(std::string configPath);

void saveGame(std::string* configPath);
bool loadGame(std::string* configPath);