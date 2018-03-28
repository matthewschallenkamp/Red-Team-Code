void four1(double *data, unsigned long nn);

extern "C" {

  void four1_interface(double *data, unsigned long nn) {
    four1(data, nn);
  }

}