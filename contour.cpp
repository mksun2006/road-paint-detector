#include "contour.h"

Contour::Contour(){
  storage = cvCreateMemStorage(0);
}

void Contour::findContours(IplImage* image){
    //copy the image
    copied_image = cvCreateImage(cvGetSize(image), 8 , 1); // 8byte/pixel 1 channel
    cvCopy(image, copied_image);
    cvClearMemStorage( storage );
    number_of_contours = cvFindContours(  copied_image, 
                                          storage, 
                                          &contours, 
                                          sizeof(CvContour), 
                                          CV_RETR_LIST, 
                                          CV_CHAIN_APPROX_NONE
                                        );
#if defined(DEBUG)
    printf("number of contours = %d\n", number_of_contours);
#endif
    printContours(contours);
}

IplImage* Contour::drawContours(){
  CvSeq* c = contours;

  for(; c != NULL; c = (CvSeq*)(c->h_next)){

    //printf("number = %d\n", c->total);
    if(c->total < 2){       
      if(c->h_prev){
        c->h_prev->h_next = c->h_next;
      } 
      else{
        contours = c->h_next;
      }
      if(c->h_next){
        c->h_next->h_prev = c->h_prev;
      }
    } 
  }
  int i =0;
  for(CvSeq* c = contours; c!=NULL; c=c->h_next){
    i++;
  }
  
  IplImage* image = cvCreateImage(cvSize(640, 480), 8 , 1);
  cvDrawContours(image, contours, cvScalar(255), cvScalar(255), 100); 

  cvNamedWindow("debug", CV_WINDOW_AUTOSIZE); 
  
  cvShowImage("debug", image);     
  printf("number of contours = %d\n", i);
  cvDrawContours(copied_image, contours, cvScalar(255), cvScalar(255), 100); 
  return copied_image;
}

void Contour::printContours(CvSeq* contours){
    int i, n= 0;
    std::ofstream out("contours.txt");
    for (CvSeq* c = contours; c!=NULL; c=c->h_next) {
      out << "Contour #" << n << std::endl;
      out << c->total << " elements:" << std::endl;
      for (i = 0; i < c->total; i++) {
        CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, c, i);        
        out << "(" << p->x << "," << p->y << ")" << std::endl; 
      }
     n++;
    }
}
