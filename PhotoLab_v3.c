/*********************************************************************/
/* PhotoLab.c: homework assignment #3, for EECS 22,  Fall 2017       */
/* Modified: Mihnea Chirila for F18, 10/30/18                                                          */
/* Author: Mina Moghadam                                                */
/* Date:   11/8/2017                                                */
/*                                                                   */
/* Comments: */
/*                                                                   */
/* Please use this template to submit your hw3.                      */
/*********************************************************************/

#include <stdio.h>
#include <string.h>

#include "DIPs.h"
#include "Advanced.h"
#include "FileIO.h"
#include "Constants.h"
#include "Test.h"
#include "Image.h"

/*** function declarations ***/

/* print a menu */
void PrintMenu();

/* Test all functions */
//void AutoTest(void);

int main()
{
    int rc ;
        

#ifdef DEBUG
    rc = AutoTest();
    if(rc != SUCCESS)
    {
        printf("AutoTest Failed, Error Code %d\n", rc);
    }

#else
    int option;			/* user input option */
    char fname[SLEN];		/* input file name */
    int brightness = -256;
    float angle;
    int opt;
    float percent;
    int st = 0;
    IMAGE *image = NULL;

    rc = 1;
    PrintMenu();
    printf("Please make your choice: ");
    scanf("%d", &option);

    while (option != EXIT) {
        if (option == 1) {
            if(st == 1){
                DeleteImage(image);
                image = NULL;
                st = 0;
                //printf("Previous image has been cleared!\n");
            }
            printf("Please input the file name to load: ");
            scanf("%s", fname);
            image = LoadImage(fname);
            if(image == NULL){
                rc = 1;
                st = 0;
            }
            else{
                rc = SUCCESS;
                st = 1;
            }
	}

		/* menu item 2 - 8 requires image is loaded first */
        else if (option >= 2 && option <= 11) {
            if (rc != SUCCESS)	 {
                printf("No image to process!\n");
                st = 0;
            }
            /* now image is loaded */
            else {
                switch (option) {
                    case 2:
                        printf("Please input the file name to save: ");
			scanf("%s", fname);
			SaveImage(fname, image);
                        st = 1;
			break;
                    case 3:
                        image = BlackNWhite(image);
                        printf("\"Black & White\" operation is done!\n");
                        st = 1;
                        break;
		    case 4:
                        image = Edge(image);
                        printf("\"Edge\" operation is done!\n");
                        st = 1;
                        break;
                    case 5:
                        image = Shuffle(image);
                        printf("\"Shuffle\" operation is done!\n");
                        st = 1;
                        break;
		    case 6:
			while(brightness > 255 || brightness < -255){
			    printf("Enter brightness value (between -255 and 255):");
			    scanf("%d", &brightness);
			}
                        image = Brightness(image, brightness);
                        printf("\"brightness\" operation is done!\n");
                        st = 1;
                        break;
		    case 7:
                        image = HMirror(image);
                        printf("\"Horizontally Mirror\" operation is done!\n");
                        st = 1;
                        break;
                    case 8:
                        printf("Enter hue rotation angle:");
                        scanf("%f", &angle);
                        image = HueRotate(image, angle);
                        printf("\"HueRotate\" operation is done!\n");
                        st = 1;
                        break;
		    case 9:
			printf("Rotate Options: \n");
                        printf(" 1: Horizontal Flip\n");
                        printf(" 2: Vertical Flip\n");
                        printf(" 3: Rotate Right\n");
                        printf(" 4: Rotate Left\n");
                        printf("Please make your choise: ");
                        scanf("%d", &opt);
                        image = Rotate(image, opt);
                        printf("\"Rotate\" operation is done!\n");
                        st = 1;
			break;
                    case 10:
                        do{
                            printf("Enter image size percentage(any number greater than 0): ");
                            scanf("%f", &percent);
                        }while(percent < 0);
                        image = Resize(image, percent);
                        printf("\"Resize\" operation is done!\n");
                        st = 1;
                        break;
                    case 11:
                        do{
                            printf("Enter saturation percentage (between -100.00 and 100.00): ");
                            scanf("%f", &percent);
                        }while(percent < -100.00 || percent > 100.00);
                        image = Saturate(image, percent);
                        printf("\"Saturate\" operation is done!\n");
                        st = 1;
                        break;
		    default:
			break;

				}
			}
		}

	else if (option == 12) {
            rc = AutoTest();
            if(rc != SUCCESS)
            {
                printf("AutoTest Failed, Error Code %d\n", rc);
            }
            st = 0;
        }
        else {
            printf("Invalid selection!\n");
        }

		/* Process finished, waiting for another input */
        PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);
	}
    if(st == 1){
        DeleteImage(image);
        image = NULL;
        st = 0;
        //printf("Image has been cleared!\n");
    }
    printf("You exit the program.\n");
#endif

	return 0;
}


/*******************************************/
/* Function implementations should go here */
/*******************************************/

/* Menu */
void PrintMenu() {
    printf("\n----------------------------\n");
    printf(" 1: Load a PPM image\n");
    printf(" 2: Save an image in PPM and JPEG format\n");
    printf(" 3: Change a color image to Black & White\n");
    printf(" 4: Sketch the edge of an image\n");
    printf(" 5: Shuffle an image\n");
    printf(" 6: Adjust the brightness of an image\n");
    printf(" 7: Mirror an image horizontally\n");
    printf(" 8: Adjust the hue of an image\n");
    printf(" 9: Rotate or flip the image\n");
    printf("10: Resize the image\n");
    printf("11: Saturate the image\n");
    printf("12: Test all functions\n");
    printf("13: Exit\n");
}


/* auto test*/
/*
void AutoTest(void) {

    char fname[SLEN] = "HSSOE";
    int brightness = 200;
    float angle = -1.5;
    IMAGE *image = NULL;

    image = LoadImage(fname);
    image = BlackNWhite(image);
    SaveImage("bw", image);
    DeleteImage(image);
    image = NULL;
    printf("Black & White tested!\n\n");

    image = LoadImage(fname);
    image = Edge(image);
    SaveImage("edge", image);
    DeleteImage(image);
    image = NULL;
    printf("Edge Detection tested!\n\n");

    image = LoadImage(fname);
    image = Shuffle(image);
    SaveImage("shuffle", image);
    DeleteImage(image);
    image = NULL; 
    printf("Shuffle tested!\n\n");

    image = LoadImage(fname);
    image = Brightness(image, brightness);
    SaveImage("brightness", image);
    DeleteImage(image);
    image = NULL;
    printf("Brightness tested!\n\n");


    image = LoadImage(fname);
    image = HMirror(image);
    SaveImage("hmirror", image);
    DeleteImage(image);
    image = NULL;
    printf("HMirror tested!\n\n");

    image = LoadImage(fname);
    image = HueRotate(image, angle);
    SaveImage("hue", image);
    DeleteImage(image);
    image = NULL;
    printf("HueRotate tested!\n\n");
}
*/
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
