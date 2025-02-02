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

/* **************** OUTPUT ******************** */

static bNodeSocketTemplate sh_node_subsurface_scattering_in[] = {
    {SOCK_RGBA, N_("Color"), 0.8f, 0.8f, 0.8f, 1.0f, 0.0f, 1.0f},
    {SOCK_FLOAT, N_("Scale"), 1.0, 0.0f, 0.0f, 0.0f, 0.0f, 1000.0f},
    {SOCK_VECTOR, N_("Radius"), 1.0f, 0.2f, 0.1f, 0.0f, 0.0f, 100.0f, PROP_NONE, SOCK_COMPACT},
    {SOCK_FLOAT, N_("IOR"), 1.4f, 0.0f, 0.0f, 0.0f, 1.01f, 3.8f, PROP_FACTOR},
    {SOCK_FLOAT, N_("Anisotropy"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_VECTOR, N_("Normal"), 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, PROP_NONE, SOCK_HIDE_VALUE},
    {-1, ""},
};

static bNodeSocketTemplate sh_node_subsurface_scattering_out[] = {
    {SOCK_SHADER, N_("BSSRDF")},
    {-1, ""},
};

static void node_shader_init_subsurface_scattering(bNodeTree *UNUSED(ntree), bNode *node)
{
  node->custom1 = SHD_SUBSURFACE_RANDOM_WALK;
  node->custom2 = true;
}

static int node_shader_gpu_subsurface_scattering(GPUMaterial *mat,
                                                 bNode *node,
                                                 bNodeExecData *UNUSED(execdata),
                                                 GPUNodeStack *in,
                                                 GPUNodeStack *out)
{
  if (!in[5].link) {
    GPU_link(mat, "world_normals_get", &in[5].link);
  }

  if (node->sss_id > 0) {
    bNodeSocket *socket = (bNodeSocket *)BLI_findlink(&node->original->inputs, 2);
    bNodeSocketValueRGBA *socket_data = (bNodeSocketValueRGBA *)socket->default_value;
    /* For some reason it seems that the socket value is in ARGB format. */
    GPU_material_sss_profile_create(mat, &socket_data->value[1]);

    /* sss_id is 0 only the node is not connected to any output.
     * In this case flagging the material would trigger a bug (see T68736). */
    GPU_material_flag_set(mat, (eGPUMatFlag)(GPU_MATFLAG_DIFFUSE | GPU_MATFLAG_SSS));
  }

  return GPU_stack_link(
      mat, node, "node_subsurface_scattering", in, out, GPU_constant(&node->sss_id));
}

static void node_shader_update_subsurface_scattering(bNodeTree *ntree, bNode *node)
{
  const int sss_method = node->custom1;

  LISTBASE_FOREACH (bNodeSocket *, sock, &node->inputs) {
    if (STR_ELEM(sock->name, "IOR", "Anisotropy")) {
      nodeSetSocketAvailability(ntree, sock, sss_method != SHD_SUBSURFACE_BURLEY);
    }
  }
}

/* node type definition */
void register_node_type_sh_subsurface_scattering(void)
{
  static bNodeType ntype;

  sh_node_type_base(
      &ntype, SH_NODE_SUBSURFACE_SCATTERING, "Subsurface Scattering", NODE_CLASS_SHADER, 0);
  node_type_socket_templates(
      &ntype, sh_node_subsurface_scattering_in, sh_node_subsurface_scattering_out);
  node_type_size_preset(&ntype, NODE_SIZE_MIDDLE);
  node_type_init(&ntype, node_shader_init_subsurface_scattering);
  node_type_storage(&ntype, "", nullptr, nullptr);
  node_type_gpu(&ntype, node_shader_gpu_subsurface_scattering);
  node_type_update(&ntype, node_shader_update_subsurface_scattering);

  nodeRegisterType(&ntype);
}
