"""Test that lldb can report the exception reason for threads in a corefile."""

import os
import re
import subprocess

import lldb
from lldbsuite.test.decorators import *
from lldbsuite.test.lldbtest import *
from lldbsuite.test import lldbutil


class TestCorefileExceptionReason(TestBase):
    @no_debug_info_test
    @skipUnlessDarwin
    @skipIf(archs=no_match(["arm64", "arm64e"]))
    @skipIfRemote
    def test(self):
        corefile = self.getBuildArtifact("process.core")
        self.build()
        (target, process, thread, bkpt) = lldbutil.run_to_source_breakpoint(
            self, "// break here", lldb.SBFileSpec("main.cpp")
        )

        self.runCmd("continue")

        self.runCmd("process save-core -s stack " + corefile)
        live_tids = []
        if self.TraceOn():
            self.runCmd("thread list")
        for t in process.threads:
            live_tids.append(t.GetThreadID())
        process.Kill()
        self.dbg.DeleteTarget(target)

        # Now load the corefile
        target = self.dbg.CreateTarget("")
        process = target.LoadCore(corefile)
        thread = process.GetSelectedThread()
        self.assertTrue(process.GetSelectedThread().IsValid())
        if self.TraceOn():
            self.runCmd("image list")
            self.runCmd("bt")
            self.runCmd("fr v")

        self.assertEqual(
            thread.stop_description, "ESR_EC_DABORT_EL0 (fault address: 0x0)"
        )

        if self.TraceOn():
            self.runCmd("thread list")
        for i in range(process.GetNumThreads()):
            t = process.GetThreadAtIndex(i)
            self.assertEqual(t.GetThreadID(), live_tids[i])
