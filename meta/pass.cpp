
#include "pass.hpp"

Eprp Pass::eprp;

void Pass::register_pass(Eprp_method &method) {
  eprp.register_method(method);

  // All the passses should start with pass.*
  assert(method.get_name().compare(0,name.size()+5,std::string{"pass." + name})==0);
}

void Pass::register_inou(Eprp_method &method) {
  // All the passses should start with inou.*
  assert(method.get_name().compare(0,name.size()+5,std::string{"inou." + name})==0);

  if(method.get_name() == std::string{"inou." + name + ".tolg"}) {
    method.add_label_optional("path","lgraph path","lgdb");
    method.add_label_required("files","input file[s]");
  }else if(method.get_name() == std::string{"inou." + name + ".fromlg"}) {
    method.add_label_optional("odir","output directory",".");
  }else if(method.get_name() == std::string{"inou." + name}) {
    method.add_label_optional("path","lgraph path","lgdb");
    method.add_label_optional("files","input file[s]");
    method.add_label_optional("odir","output directory",".");
  }else{
    assert(false);
    // inou methods should be inou.name.tolg or inou.name.fromlg or inou.name generic for passes that handle one way only
    //
    // Possible to have submothods like inou.name.tolg.foobar
  }

  eprp.register_method(method);
}

bool Pass::setup_directory(const std::string &dir) const {

  if (dir == ".")
    return true;

  struct stat sb;

  if (stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    return true;

  int e = mkdir(dir.c_str(),0755);
  if (e<0) {
    error(fmt::format("could not create directory {}",dir));
    return false;
  }

  return true;
}

Pass::Pass(const std::string &name_)
  :name(name_) {
  };
