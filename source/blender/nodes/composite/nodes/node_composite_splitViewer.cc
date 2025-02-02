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
 * The Original Code is Copyright (C) 2006 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup cmpnodes
 */

#include "node_composite_util.hh"

#include "BKE_global.h"
#include "BKE_image.h"

/* **************** SPLIT VIEWER ******************** */

namespace blender::nodes {

static void cmp_node_split_viewer_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Color>(N_("Image"));
  b.add_input<decl::Color>(N_("Image"), "Image_001");
}

}  // namespace blender::nodes

static void node_composit_init_splitviewer(bNodeTree *UNUSED(ntree), bNode *node)
{
  ImageUser *iuser = (ImageUser *)MEM_callocN(sizeof(ImageUser), "node image user");
  node->storage = iuser;
  iuser->sfra = 1;
  node->custom1 = 50; /* default 50% split */

  node->id = (ID *)BKE_image_ensure_viewer(G.main, IMA_TYPE_COMPOSITE, "Viewer Node");
}

void register_node_type_cmp_splitviewer(void)
{
  static bNodeType ntype;

  cmp_node_type_base(
      &ntype, CMP_NODE_SPLITVIEWER, "Split Viewer", NODE_CLASS_OUTPUT, NODE_PREVIEW);
  ntype.declare = blender::nodes::cmp_node_split_viewer_declare;
  node_type_init(&ntype, node_composit_init_splitviewer);
  node_type_storage(&ntype, "ImageUser", node_free_standard_storage, node_copy_standard_storage);

  ntype.no_muting = true;

  nodeRegisterType(&ntype);
}
