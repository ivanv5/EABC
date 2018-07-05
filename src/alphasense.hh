#ifndef ALPHASENSE_HH
#define ALPHASENSE_HH

# include "ads1118.hh"

//class alphasense{

//public:
  double conversion(int16_t oz1){
      int lm= oz1;
     double  lm1= ((lm*4.096))/32768;
      return lm1;
//  }
//private:
      //double lm1;

};
  

#endif

 
