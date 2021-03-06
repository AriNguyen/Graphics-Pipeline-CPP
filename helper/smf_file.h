/**
 * @file ps_image.hpp
 * @brief C++ Program hold PSImage class that returns points, lines, polygons in .ps iamge
 * @author Ari Nguyen
 *
 */
#pragma once

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "utils.h"
#include "geometry_objects.h"

class SMFImage {
    private:
        char *imagePath;
        std::map<int, vec4D> vertex; //counter-clockwise order
        std::vector<triangle> face;

    public:
        SMFImage() {};
        SMFImage(char* i) {
            imagePath = i;
        }

        void parseData() {
            std::string line;
            std::ifstream ifs(imagePath);
            if (!ifs.is_open()) {
                std::cout << "Error: could not open file " << imagePath << "\n";
                exit(EXIT_FAILURE);
            }
            int i = 0;
            while (std::getline(ifs, line)) {
                // tokenize
                std::vector<std::string> tokens = tokenizeBySymbol(line, ' ');
                // fprintf(stderr, "line: %s\n", line.c_str());
                // fprintf(stderr, "tokens: %s\n", tokens[0].c_str());
                if (tokens.size() == 0) 
                    continue;
                assert(tokens.size() == 4);

                if (tokens[0] == "v") {
                    i++;
                    vec4D v(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
                    vertex[i] = v;
                }
                else if (tokens[0] == "f") {
                    // std::cerr << "vertex size: " << vertex.size() << "\n";
                    // std::cerr << "SMFImage::parseData tokens: " << tokens[1] << ", " << tokens[2] << ", " << tokens[3] << "'\n";
                    vec4D v1 = vertex[std::stoi(tokens[1])];
                    vec4D v2 = vertex[std::stoi(tokens[2])];
                    vec4D v3 = vertex[std::stoi(tokens[3])];
                    face.push_back(triangle(v1, v2, v3));
                }
            }
            ifs.close();
        }

        std::vector<triangle> getTriangularFace() {
            return face;
        }
        std::map<int, vec4D> getVertex() {
            return vertex;
        }
};

