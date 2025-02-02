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

#include "BLI_hash.h"

/* **************** OUTPUT ******************** */

static bNodeSocketTemplate sh_node_output_aov_in[] = {
    {SOCK_RGBA, N_("Color"), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {SOCK_FLOAT, N_("Value"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    {-1, ""},
};

static void node_shader_init_output_aov(bNodeTree *UNUSED(ntree), bNode *node)
{
  NodeShaderOutputAOV *aov = (NodeShaderOutputAOV *)MEM_callocN(sizeof(NodeShaderOutputAOV),
                                                                "NodeShaderOutputAOV");
  node->storage = aov;
}

static int node_shader_gpu_output_aov(GPUMaterial *mat,
                                      bNode *node,
                                      bNodeExecData *UNUSED(execdata),
                                      GPUNodeStack *in,
                                      GPUNodeStack *out)
{
  GPUNodeLink *outlink;
  NodeShaderOutputAOV *aov = (NodeShaderOutputAOV *)node->storage;
  /* Keep in sync with `renderpass_lib.glsl#render_pass_aov_hash` and
   * `EEVEE_renderpasses_aov_hash`. */
  unsigned int hash = BLI_hash_string(aov->name) << 1;
  GPU_stack_link(mat, node, "node_output_aov", in, out, &outlink);
  GPU_material_add_output_link_aov(mat, outlink, hash);

  return true;
}

/* node type definition */
void register_node_type_sh_output_aov(void)
{
  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_OUTPUT_AOV, "AOV Output", NODE_CLASS_OUTPUT, 0);
  node_type_socket_templates(&ntype, sh_node_output_aov_in, nullptr);
  node_type_init(&ntype, node_shader_init_output_aov);
  node_type_storage(
      &ntype, "NodeShaderOutputAOV", node_free_standard_storage, node_copy_standard_storage);
  node_type_gpu(&ntype, node_shader_gpu_output_aov);

  ntype.no_muting = true;

  nodeRegisterType(&ntype);
}
