# SPDX-License-Identifier: GPL-2.0-or-later
import frrtest


class TestBgpGrEpoch(frrtest.TestMultiOut):
    program = "./test_bgp_gr_epoch"


TestBgpGrEpoch.onesimple("BGP GR epoch retention scenarios passed.")
