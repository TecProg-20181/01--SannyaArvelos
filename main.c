#include <stdio.h>
#include <stdbool.h>

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512
#define IMAGE_RGB 3
#define RED 0
#define GREEN 1
#define BLUE 2
#define MAXIMUM_RGB 255
#define GRAY_SCALE_TRANSFORMATION 1
#define SEPIA_FILTER_TRANSFORMATION 2
#define BLUR_FILTER_TRANSFORMATION 3
#define ROTATION_TRANSFORMATION 4
#define MIRRORING_FILTER_TRANSFORMATION 5
#define INVERT_COLOR_TRANSFORMATION 6
#define CUT_IMAGE_TRANSFORMATION 7

typedef struct _pixel {
  unsigned short int red;
  unsigned short int green;
  unsigned short int blue;
} Pixel;

typedef struct _image {
  Pixel pixel[IMAGE_HEIGHT][IMAGE_WIDTH];
  unsigned int width;
  unsigned int height;
} Image;

int maximum_value(int value_a, int value_b) {
    if (value_a > value_b) {
      return value_a;
    } else {
      return value_b;
    }
}

bool pixel_igual(Pixel pixel_a, Pixel pixel_b) {
      if (pixel_a.red == pixel_b.red &&
        pixel_a.green == pixel_b.green &&
        pixel_a.blue == pixel_b.blue) {
        return true;
      } else{
        return false;
      }
}

Image gray_scale(Image image) {
  for (unsigned int image_row = 0; image_row < image.height; ++image_row) {
      for (unsigned int image_column = 0; image_column < image.width; ++image_column) {

          int media = image.pixel[image_row][image_column].red +
                      image.pixel[image_row][image_column].green +
                      image.pixel[image_row][image_column].blue;
          media /= 3;
          image.pixel[image_row][image_column].red = media;
          image.pixel[image_row][image_column].green = media;
          image.pixel[image_row][image_column].blue = media;
      }
  }
  return image;
}

int mininum_blur_height(int image_height, int image_row, int blur_size) {
  int half_blur_size = blur_size/2;
  if (image_height - 1 > image_row + half_blur_size) {
    return image_row + half_blur_size;
  } else {
    return image_height - 1;
  }
}

int mininum_blur_width(int image_width, int image_column, int blur_size) {
  int half_blur_size = blur_size/2;
  if (image_width - 1 > image_column + half_blur_size) {
    return image_column + half_blur_size;
  } else {
    return image_width - 1;
  }
}

Image blur_filter(Image image, int blur_size) {
    for (unsigned int image_row = 0; image_row < image.height; ++image_row) {
        for (unsigned int image_column = 0; image_column < image.width; ++image_column) {
            Pixel media_pixel = {0, 0, 0};

            // Achar um nome legal
            int minimum_height = mininum_blur_height(image.height, image_row, blur_size);
            int minimum_width = mininum_blur_width(image.width, image_column, blur_size);

            int starting_image_row = (0 > image_row - blur_size/2) ? 0 : image_row - blur_size/2;
            int starting_image_column = (0 > image_column - blur_size/2) ? 0 : image_column - blur_size/2;

            for (starting_image_row; starting_image_row <= minimum_height; ++starting_image_row) {
                for (starting_image_column; starting_image_column <= minimum_width; ++starting_image_column) {
                    Pixel applying_pixel_blur = image.pixel[starting_image_row][starting_image_column];
                    media_pixel.red += applying_pixel_blur.red;
                    media_pixel.green += applying_pixel_blur.green;
                    media_pixel.blue += applying_pixel_blur.blue;
                }
            }

            int media_blur_size_pixel = blur_size * blur_size;
            media_pixel.red /= media_blur_size_pixel;
            media_pixel.green /= media_blur_size_pixel;
            media_pixel.blue /= media_blur_size_pixel;

            image.pixel[image_row][image_column] = media_pixel;
            //Pixel *image_pixel = &image.pixel[image_row][image_column];
            //image_pixel->red = media_pixel.red;
        }
    }
    return image;
}

Image rotate_90_to_right(Image image) {
  Image rotated_image;

  rotated_image.width = image.height;
  rotated_image.height = image.width;

  unsigned int image_column = rotated_image.width - 1;

  for (unsigned int image_row = 0, rotated_image_column = 0; image_row < rotated_image.height; ++image_row, ++rotated_image_column) {
    for (unsigned int image_column, rotated_image_row = 0; image_column >= 0; --image_column, ++rotated_image_row) {
      // Pixel *pixel = image.pixel[rotated_image_row, rotated_image_column];
      // Pixel *rotated_pixel = &rotated_image.pixel[image_row, image_column];
      //
      // rotated_pixel->red = pixel.red;
      // rotated_pixel->green = pixel.green;
      // rotated_pixel->blue = pixel.blue;

      rotated_image.pixel[image_row][image_column].red = image.pixel[rotated_image_row][rotated_image_column].red;
      rotated_image.pixel[image_row][image_column].green = image.pixel[rotated_image_row][rotated_image_column].green;
      rotated_image.pixel[image_row][image_column].blue = image.pixel[rotated_image_row][rotated_image_column].blue;
    }
  }
  return rotated_image;
}

Image rotate_image(Image image) {
  int rotation, times_to_rotate = 0;

  scanf("%d", &times_to_rotate);
  times_to_rotate %= 4;

  for (rotation; rotation < times_to_rotate; ++rotation) {
      image = rotate_90_to_right(image);
  }
  return image;
}

Image invert_image_color(Image image) {
  for (unsigned int image_row = 0; image_row < image.height; ++image_row) {
    for (unsigned int image_column = 0; image_column < image.width; ++image_column) {

      Pixel *pixel = &image.pixel[image_row][image_column];

      pixel->red = MAXIMUM_RGB - pixel->red;
      pixel->green = MAXIMUM_RGB - pixel->green;
      pixel->blue = MAXIMUM_RGB - pixel->blue;
    }
  }
  return image;
}

// Segmentation fault
Image cut_image(Image image,
  int cut_from_image_column, int cut_from_image_row,
  int cut_to_image_column, int cut_to_image_row) {
    Image cutted_image;

    cutted_image.width = cut_to_image_column;
    cutted_image.height = cut_to_image_row;

    for(unsigned int image_row = 0; image_row < cut_to_image_row; ++image_row) {
        for(unsigned int image_column = 0; image_column < cut_to_image_column; ++image_column) {
            Pixel image_pixel = image.pixel[image_row+cut_from_image_column][image_column+cut_to_image_column];
            Pixel *cutted_image_pixel = &cutted_image.pixel[image_row][image_column];
            cutted_image_pixel->red = image_pixel.red;
            cutted_image_pixel->green = image_pixel.green;
            cutted_image_pixel->blue = image_pixel.blue;
        }
    }

    return cutted_image;
}

int final_sepia_pixel(sepia_pixel) {
  if (MAXIMUM_RGB > sepia_pixel) {
    return sepia_pixel;
  } else {
    return MAXIMUM_RGB;
  }
}

Image sepia_filter(Image image) {
  for (unsigned int image_row = 0; image_row < image.height; ++image_row) {
      for (unsigned int image_column = 0; image_column < image.width; ++image_column) {
          Pixel current_pixel = image.pixel[image_row][image_column];
          Pixel *pixel = &image.pixel[image_row][image_column];

          int sepia_pixel;

          sepia_pixel = current_pixel.red * .393 + current_pixel.green * .769 + current_pixel.blue * .189;
          pixel->red = final_sepia_pixel(sepia_pixel);

          sepia_pixel =  current_pixel.red * .349 + current_pixel.green * .686 + current_pixel.blue * .168;
          pixel->green = final_sepia_pixel(sepia_pixel);

          sepia_pixel =  current_pixel.red * .272 + current_pixel.green * .534 + current_pixel.blue * .131;
          pixel->blue = final_sepia_pixel(sepia_pixel);
      }
  }
  return image;
}

Image apply_transformation(Image image, int transformation_option) {
  switch(transformation_option) {
    case GRAY_SCALE_TRANSFORMATION: {
          image = gray_scale(image);
          break;
      }
      case SEPIA_FILTER_TRANSFORMATION: {
          image = sepia_filter(image);
          break;
      }
      case BLUR_FILTER_TRANSFORMATION: {
          int blur_size = 0;
          scanf("%d", &blur_size);
          image = blur_filter(image, blur_size);
          break;
      }
      case ROTATION_TRANSFORMATION: {
          image = rotate_image(image);
          break;
      }
      // case MIRRORING_FILTER_TRANSFORMATION: {
      //     int horizontal = 0;
      //     scanf("%d", &horizontal);
      //
      //     int w = img.w, h = img.h;
      //
      //     if (horizontal == 1) w /= 2;
      //     else h /= 2;
      //
      //     for (int i2 = 0; i2 < h; ++i2) {
      //         for (int j = 0; j < w; ++j) {
      //             int x = i2, y = j;
      //
      //             if (horizontal == 1) y = img.w - 1 - j;
      //             else x = img.h - 1 - i2;
      //
      //             Pixel aux1;
      //             aux1.r = img.pixel[i2][j][0];
      //             aux1.g = img.pixel[i2][j][1];
      //             aux1.b = img.pixel[i2][j][2];
      //
      //             img.pixel[i2][j][0] = img.pixel[x][y][0];
      //             img.pixel[i2][j][1] = img.pixel[x][y][1];
      //             img.pixel[i2][j][2] = img.pixel[x][y][2];
      //
      //             img.pixel[x][y][0] = aux1.r;
      //             img.pixel[x][y][1] = aux1.g;
      //             img.pixel[x][y][2] = aux1.b;
      //         }
      //     }
      //     break;
      // }
      case INVERT_COLOR_TRANSFORMATION: {
          image = invert_image_color(image);
          break;
      }
      case CUT_IMAGE_TRANSFORMATION: {
          int cut_from_image_column, cut_from_image_row;
          scanf("%d %d", &cut_from_image_column, &cut_from_image_row);
          int cut_to_image_column, cut_to_image_row;
          scanf("%d %d", &cut_to_image_column, &cut_to_image_row);
          image = cut_image(image, cut_from_image_column, cut_from_image_row, cut_to_image_column, cut_to_image_row);
          break;
      }
  }
  return image;
}

int main() {
    Image image;

    // read type of image
    char image_type[4];
    scanf("%s", image_type);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &image.width, &image.height, &max_color);

    // read all pixels of image
    for (unsigned int image_row = 0; image_row < image.height; ++image_row) {
        for (unsigned int image_column = 0; image_column < image.width; ++image_column) {
            Pixel *pixel = &image.pixel[image_row][image_column];
            scanf("%hu %hu %hu", &pixel->red, &pixel->green, &pixel->blue);
        }
    }

    int options;
    scanf("%d", &options);

    for(int option = 0; option < options; ++option) {
        int transformation_option;
        scanf("%d", &transformation_option);
        image = apply_transformation(image, transformation_option);
    }

    // print type of image
    printf("P3\n");

    // print width, height and color of image
    printf("%u %u\n255\n", image.width, image.height);

    // print pixels of image
    for (unsigned int image_row = 0; image_row < image.height; ++image_row) {
        for (unsigned int image_column = 0; image_column < image.width; ++image_column) {
            Pixel pixel = image.pixel[image_row][image_column];
            printf("%hu %hu %hu ", pixel.red, pixel.green, pixel.blue);
        }
        printf("\n");
    }
    return 0;
}
