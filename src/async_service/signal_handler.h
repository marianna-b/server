#ifndef SIGNAL_HANDLER
#define SIGNAL_HANDLER

namespace tcp {
   const int MAX_SERVICES = 100;

   struct signal_handler {

      static void add(int);
      static void set();
      static void run_handler(int);

   private:
      volatile static int size;

      volatile static int services[MAX_SERVICES];
   };

}

#endif // SIGNAL_HANDLER
