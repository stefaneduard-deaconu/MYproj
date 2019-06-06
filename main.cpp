#include "path.h"
#include "char_star.h"
#include "console.h"
#include "dict_class.h"
#include "app_class.h"

using namespace std;

int main()
{

  // here we'll instantiate and work with the App class
    App* application = App::get_instance();
    application->start();
  //

  return 0;
}
