/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup shdnodes
 */

#include "node_shader_util.hh"

/* **************** Script ******************** */

static void init(bNodeTree *UNUSED(ntree), bNode *node)
{
  NodeShaderScript *nss = (NodeShaderScript *)MEM_callocN(sizeof(NodeShaderScript),
                                                          "shader script node");
  node->storage = nss;
}

static void node_free_script(bNode *node)
{
  NodeShaderScript *nss = (NodeShaderScript *)node->storage;

  if (nss) {
    if (nss->bytecode) {
      MEM_freeN(nss->bytecode);
    }

    MEM_freeN(nss);
  }
}

static void node_copy_script(bNodeTree *UNUSED(dest_ntree),
                             bNode *dest_node,
                             const bNode *src_node)
{
  NodeShaderScript *src_nss = (NodeShaderScript *)src_node->storage;
  NodeShaderScript *dest_nss = (NodeShaderScript *)MEM_dupallocN(src_nss);

  if (src_nss->bytecode) {
    dest_nss->bytecode = (char *)MEM_dupallocN(src_nss->bytecode);
  }

  dest_node->storage = dest_nss;
}

void register_node_type_sh_script(void)
{
  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_SCRIPT, "Script", NODE_CLASS_SCRIPT, 0);
  node_type_init(&ntype, init);
  node_type_storage(&ntype, "NodeShaderScript", node_free_script, node_copy_script);

  nodeRegisterType(&ntype);
}
