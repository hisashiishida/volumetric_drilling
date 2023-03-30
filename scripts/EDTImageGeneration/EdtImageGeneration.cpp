#include "EdtReader.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "lodepng.h"

using namespace std;

void encodeTwoSteps(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
  std::vector<unsigned char> png;

  unsigned error = lodepng::encode(png, image, width, height);
  if(!error) lodepng::save_file(png, filename);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

int main(int argc, char* argv[])
{
    cout << "Edt Image generation example\n";

    if (argc != 3){
        cerr << "[ERROR] Missing arguments. You have to specify two arguments, (1)path to .edt file and (2)output file." << endl;
        return 1;
    }
    
    char input_file_name[100];
    sprintf(input_file_name, argv[1]);
    
    float *values_buffer;
    unsigned int res[3];

    Array3d<float> *edt_grid;
    edt_reader(input_file_name, &values_buffer, res);

    edt_grid = new Array3d<float>(values_buffer, res);

    cout << "Resolution:" << res[0] << ","  << res[1] << "," << res[2] << endl;

    unsigned width = res[0], height = res[1];
    std::vector<unsigned char> image;
    image.resize(width * height * 4);
    int z=0;
    double max_edt = 0;
    int scale = sqrt(res[0]*res[0] + res[1]*res[1]);//Scaling down the edt pixel distance 
    for (z = 0; z < res[2]; z++){
        for(unsigned y = 0; y < height; y++){
            for(unsigned x = 0; x < width; x++) {
                double tmp = (*edt_grid)(x,y,z) * 255/scale;
                if (tmp > max_edt) max_edt =tmp/255 * scale;
                if (tmp > 255) cout << "[ERROR] edt distance over "<< scale  << " mm(" << max_edt << ")" << endl;

                if (tmp > 0) image[4 * width * y + 4 * x + 0] = tmp; //R
                if (tmp < 0) image[4 * width * y + 4 * x + 2] = -tmp; //B
                // image[4 * width * y + 4 * x + 2] = 0; //G
                image[4 * width * y + 4 * x + 3] = 255;  //A
            }
        }

        // Create imgaes in the output file
        string index = to_string(z);

        char out_filename[200];
        sprintf(out_filename, "./%s/edtplane_%s.png", argv[2], &index[0]);
        // cout << out_filename << endl;

        encodeTwoSteps(out_filename, image, width, height);
    }

    
    return 0;
}