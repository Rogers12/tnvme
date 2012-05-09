/*
 * Copyright (c) 2011, Intel Corporation.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "unsupportRsvdFields_r10b.h"
#include "globals.h"
#include "grpDefs.h"
#include "../Queues/iocq.h"
#include "../Queues/iosq.h"
#include "../Utils/io.h"
#include "../Cmds/flush.h"

namespace GrpNVMFlushCmd {


UnsupportRsvdFields_r10b::UnsupportRsvdFields_r10b(int fd, string mGrpName,
    string mTestName, ErrorRegs errRegs) :
    Test(fd, mGrpName, mTestName, SPECREV_10b, errRegs)
{
    // 63 chars allowed:     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    mTestDesc.SetCompliance("revision 1.0b, section 6");
    mTestDesc.SetShort(     "Set unsupported/rsvd fields in cmd");
    // No string size limit for the long description
    mTestDesc.SetLong(
        "Unsupported DW's and rsvd fields are treated identical, the recipient "
        "shall not check their value. Determine Identify.NN and issue flush "
        "cmd to all namspc, expect success. Then issue same cmd setting all "
        "unsupported/rsvd fields, expect success. Set: DW0_b15:10, DW2, DW3, "
        "DW4, DW5, DW6, DW7, DW8, DW9, DW10, DW11, DW12, DW13, DW14, DW15.");
}


UnsupportRsvdFields_r10b::~UnsupportRsvdFields_r10b()
{
    ///////////////////////////////////////////////////////////////////////////
    // Allocations taken from the heap and not under the control of the
    // RsrcMngr need to be freed/deleted here.
    ///////////////////////////////////////////////////////////////////////////
}


UnsupportRsvdFields_r10b::
UnsupportRsvdFields_r10b(const UnsupportRsvdFields_r10b &other) : Test(other)
{
    ///////////////////////////////////////////////////////////////////////////
    // All pointers in this object must be NULL, never allow shallow or deep
    // copies, see Test::Clone() header comment.
    ///////////////////////////////////////////////////////////////////////////
}


UnsupportRsvdFields_r10b &
UnsupportRsvdFields_r10b::operator=(const UnsupportRsvdFields_r10b &other)
{
    ///////////////////////////////////////////////////////////////////////////
    // All pointers in this object must be NULL, never allow shallow or deep
    // copies, see Test::Clone() header comment.
    ///////////////////////////////////////////////////////////////////////////
    Test::operator=(other);
    return *this;
}


void
UnsupportRsvdFields_r10b::RunCoreTest()
{
    /** \verbatim
     * Assumptions:
     * 1) Test CreateResources_r10b has run prior.
     * \endverbatim
     */

    // Lookup objs which were created in a prior test within group
    SharedIOSQPtr iosq = CAST_TO_IOSQ(gRsrcMngr->GetObj(IOSQ_GROUP_ID));
    SharedIOCQPtr iocq = CAST_TO_IOCQ(gRsrcMngr->GetObj(IOCQ_GROUP_ID));

    SharedFlushPtr flushCmd = SharedFlushPtr(new Flush());
    flushCmd->SetNSID(1);

    IO::SendAndReapCmd(mGrpName, mTestName, DEFAULT_CMD_WAIT_ms, iosq, iocq,
        flushCmd, "none.set", true);

    LOG_NRM("Set all cmd's rsvd bits");
    uint32_t work = flushCmd->GetDword(0);
    work |= 0x0000fc00;      // Set DW0_b15:10 bits
    flushCmd->SetDword(work, 0);

    flushCmd->SetDword(0xffff, 2);
    flushCmd->SetDword(0xffff, 3);
    flushCmd->SetDword(0xffff, 4);
    flushCmd->SetDword(0xffff, 5);
    flushCmd->SetDword(0xffff, 6);
    flushCmd->SetDword(0xffff, 7);
    flushCmd->SetDword(0xffff, 8);
    flushCmd->SetDword(0xffff, 9);
    flushCmd->SetDword(0xffff, 10);
    flushCmd->SetDword(0xffff, 11);
    flushCmd->SetDword(0xffff, 12);
    flushCmd->SetDword(0xffff, 13);
    flushCmd->SetDword(0xffff, 14);
    flushCmd->SetDword(0xffff, 15);

    IO::SendAndReapCmd(mGrpName, mTestName, DEFAULT_CMD_WAIT_ms, iosq, iocq,
        flushCmd, "all.set", true);
}


}   // namespace
