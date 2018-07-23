#  This file is distributed under the BSD 3-Clause License. See LICENSE for details.
import sys
import os
import shutil

base_dir = os.path.dirname(sys.argv[0]) or '.'
sys.path.insert(1, base_dir)
package_dir_a = os.path.join(base_dir, '__main__')
sys.path.insert(1, package_dir_a)

print("test running directory: ", os.getcwd())
shutil.rmtree("lgdb", ignore_errors=True)  # remove previous garbage

try:
  import lgraph

  cfg_name = 'pt_simple_add'
  # cfg_name = 'pt1'
  dfg_name = cfg_name + '_dfg'

  cfg_opts = {
        "lgdb": "lgdb",
        "graph_name": cfg_name,
        "src": "inou/cfg/tests/%s.cfg" % cfg_name
        }

  sys.stdout.flush()
  cfg = lgraph.Inou_cfg(cfg_opts).generate()
  sys.stdout.flush()
  print("===================== cfg pass ======================")

  dfg_opts = {
        "lgdb": "lgdb",
        "src": cfg_name,
        "graph_name": dfg_name
        }

  dfg = lgraph.Pass_dfg(dfg_opts).generate()

  assert not (dfg is None)
  assert not (dfg[0] is None)
  sys.stdout.flush()
  print("===================== dfg pass ======================")


  g = lgraph.find_lgraph("lgdb",dfg_name)
  assert dfg[0].lg_id() == g.lg_id()
  g.dump()
  print("==================== dump pass ======================")



except:
  print("lgraph raised exception. Test fails")
  assert(False)


