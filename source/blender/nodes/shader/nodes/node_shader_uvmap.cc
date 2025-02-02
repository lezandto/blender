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

#include "node_shader_util.hh"

#include "DNA_customdata_types.h"

/* **************** OUTPUT ******************** */

static bNodeSocketTemplate sh_node_uvmap_out[] = {
    {SOCK_VECTOR, N_("UV"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {-1, ""},
};

static void node_shader_init_uvmap(bNodeTree *UNUSED(ntree), bNode *node)
{
  NodeShaderUVMap *attr = (NodeShaderUVMap *)MEM_callocN(sizeof(NodeShaderUVMap),
                                                         "NodeShaderUVMap");
  node->storage = attr;
}

static int node_shader_gpu_uvmap(GPUMaterial *mat,
                                 bNode *node,
                                 bNodeExecData *UNUSED(execdata),
                                 GPUNodeStack *in,
                                 GPUNodeStack *out)
{
  NodeShaderUVMap *attr = (NodeShaderUVMap *)node->storage;
  GPUNodeLink *mtface = GPU_attribute(mat, CD_MTFACE, attr->uv_map);

  GPU_stack_link(mat, node, "node_uvmap", in, out, mtface);

  node_shader_gpu_bump_tex_coord(mat, node, &out[0].link);

  return 1;
}

/* node type definition */
void register_node_type_sh_uvmap(void)
{
  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_UVMAP, "UV Map", NODE_CLASS_INPUT, 0);
  node_type_socket_templates(&ntype, nullptr, sh_node_uvmap_out);
  node_type_size_preset(&ntype, NODE_SIZE_MIDDLE);
  node_type_init(&ntype, node_shader_init_uvmap);
  node_type_storage(
      &ntype, "NodeShaderUVMap", node_free_standard_storage, node_copy_standard_storage);
  node_type_gpu(&ntype, node_shader_gpu_uvmap);

  nodeRegisterType(&ntype);
}
