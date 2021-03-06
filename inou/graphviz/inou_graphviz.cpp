//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.
//

#include <fstream>
#include <atomic>

#include "lgbench.hpp"
#include "lgedgeiter.hpp"
#include "lgraphbase.hpp"
#include "eprp_utils.hpp"

#include "inou_graphviz.hpp"


void setup_inou_graphviz() {
  Inou_graphviz p;
  p.setup();
}

void Inou_graphviz::setup() {

  Eprp_method m2("inou.graphviz", "export lgraph to graphviz dot format", &Inou_graphviz::fromlg);

  register_inou(m2);
}

Inou_graphviz::Inou_graphviz()
 :Pass("graphviz") {
}

void Inou_graphviz::fromlg(Eprp_var &var) {

  Inou_graphviz p;

  p.odir = var.get("odir");
  bool ok = p.setup_directory(p.odir);
  if (!ok)
    return;

  std::vector<const LGraph *> lgs;
  for(const auto &l:var.lgs) {
    lgs.push_back(l);
  }

  p.do_fromlg(lgs);
}

void Inou_graphviz::do_fromlg(std::vector<const LGraph *> &lgs) {

  for(const auto g:lgs) {

    std::string data = "digraph {\n";

    g->each_master_root_fast([g,&data](Index_ID src_nid) {
      const auto &node = g->node_type_get(src_nid);
      data += fmt::format(" {} [label=\"{}:{}\"];\n", src_nid, src_nid, node.get_name());
    });

    g->each_output_edge_fast([&data](Index_ID src_nid, Port_ID src_pid, Index_ID dst_nid, Port_ID dst_pid) {
      data += fmt::format(" {} -> {}[label=\"{}:{}\"];\n", src_nid, dst_nid, src_pid, dst_pid);
    });
    data += "}\n";

    std::string file = odir + "/" + g->get_name() + ".dot";
    int fd = ::open(file.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd<0) {
      console->error("inou.graphviz unable to create {}", file);
      return;
    }
    write(fd,data.c_str(),data.size());
    close(fd);
  }
}

