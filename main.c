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

bool is_pixel_equals(Pixel pixel_a, Pixel pixel_b) {
  if (pixel_a.red == pixel_b.red &&
    pixel_a.green == pixel_b.green &&
    pixel_a.blue == pixel_b.blue) {
    return true;
  } else {
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

int mininum_image_blur_pixel(int image_size, int pixel_unit, int blur_size) {
  int main_minimum_pixel = pixel_unit + blur_size/2;
  int alternative_minimum_pixel = image_size - 1;
  if (main_minimum_pixel < alternative_minimum_pixel) {
    return main_minimum_pixel;
  } else {
    return alternative_minimum_pixel;
  }
}

int initial_image_blur_pixel(int pixel_position, int blur_size) {
  int initial_image_pixel = pixel_position - blur_size/2;
  if (initial_image_pixel < 0) {
    return 0;
  } else {
    return initial_image_pixel;
  }
}

Image blur_filter(Image image, int blur_size) {
  for (unsigned int image_row = 0; image_row < image.height; ++image_row) {
    for (unsigned int image_column = 0; image_column < image.width; ++image_column) {
      Pixel media_pixel = {0, 0, 0};

      int minimum_height = mininum_image_blur_pixel(image.height, image_row, blur_size);
      int minimum_width = mininum_image_blur_pixel(image.width, image_column, blur_size);

      for (
        int starting_image_row = initial_image_blur_pixel(image_row, blur_size);
        starting_image_row <= minimum_height;
        ++starting_image_row
      ) {
        for (
          int starting_image_column = initial_image_blur_pixel(image_column, blur_size);
          starting_image_column <= minimum_width;
          ++starting_image_column
        ) {
          Pixel blur_pixel = image.pixel[starting_image_row][starting_image_column];
          media_pixel.red += blur_pixel.red;
          media_pixel.green += blur_pixel.green;
          media_pixel.blue += blur_pixel.blue;
        }
      }

      int media_blur_size_pixel = blur_size * blur_size;
      media_pixel.red /= media_blur_size_pixel;
      media_pixel.green /= media_blur_size_pixel;
      media_pixel.blue /= media_blur_size_pixel;

      image.pixel[image_row][image_column] = media_pixel;
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

      rotated_image.pixel[image_row][image_column].red = image.pixel[rotated_image_row][rotated_image_column].red;
      rotated_image.pixel[image_row][image_column].green = image.pixel[rotated_image_row][rotated_image_column].green;
      rotated_image.pixel[image_row][image_column].blue = image.pixel[rotated_image_row][rotated_image_column].blue;
    }
  }
  return rotated_image;
}

Image rotate_image(Image image) {
  int times_to_rotate = 0;

  scanf("%d", &times_to_rotate);
  times_to_rotate %= 4;

  for (int rotation = 0; rotation < times_to_rotate; ++rotation) {
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
Image cut_image(Image image, int cut_from_image_column, int cut_from_image_row, int cut_image_column, int cut_image_row) {
  Image cutted_image;
  cutted_image.width = cut_image_column;
  cutted_image.height = cut_image_row;

  for(int image_row = 0; image_row < cut_image_row; ++image_row) {
    for(int image_column = 0; image_column < cut_image_column; ++image_column) {

      int row_pixel = image_row + cut_from_image_row;
      int column_pixel = image_column + cut_from_image_column;

      cutted_image.pixel[image_row][image_column] = image.pixel[row_pixel][column_pixel];
    }
  }

  return cutted_image;
}

int final_sepia_pixel(int sepia_pixel) {
  if (MAXIMUM_RGB > sepia_pixel) {
    return sepia_pixel;
  } else {
    return MAXIMUM_RGB;
  }
}

Image mirror_image(Image image) {
  int horizontal = 0;

  scanf("%d", &horizontal);

  int width = image.width, height = image.height;

  if (horizontal == 1) {
    width /= 2;
  }
  else {
    height /= 2;
  }

  for (int image_row = 0; image_row < height; ++image_row) {
    for (int image_column = 0; image_column < width; ++image_column) {
      int mirrored_image_row = image_row;
      int mirrored_image_column = image_column;

      if (horizontal == 1) {
        mirrored_image_column = image.width - 1 - image_column;
      } else {
        mirrored_image_row = image.height - 1 - image_row;
      }

      Pixel pixel = image.pixel[image_row][image_column];
      image.pixel[image_row][image_column] = image.pixel[mirrored_image_row][mirrored_image_column];
      image.pixel[mirrored_image_row][mirrored_image_column] = pixel;
    }
  }
  return image;
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
      case MIRRORING_FILTER_TRANSFORMATION: {
          image = mirror_image(image);
          break;
      }
      case INVERT_COLOR_TRANSFORMATION: {
          image = invert_image_color(image);
          break;
      }
      case CUT_IMAGE_TRANSFORMATION: {
          int cut_from_image_column, cut_from_image_row;
          scanf("%d %d", &cut_from_image_column, &cut_from_image_row);

          int cut_image_column, cut_image_row;
          scanf("%d %d", &cut_image_column, &cut_image_row);

          image = cut_image(image, cut_from_image_column, cut_from_image_row, cut_image_column, cut_image_row);
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
