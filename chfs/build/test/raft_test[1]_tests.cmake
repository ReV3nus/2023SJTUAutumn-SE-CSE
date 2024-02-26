add_test( RaftTestPart1.LeaderElection /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart1.LeaderElection]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart1.LeaderElection PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart1.ReElection /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart1.ReElection]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart1.ReElection PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart2.BasicAgree /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart2.BasicAgree]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart2.BasicAgree PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart2.FailAgreement /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart2.FailAgreement]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart2.FailAgreement PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart2.FailNoAgreement /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart2.FailNoAgreement]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart2.FailNoAgreement PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart2.ConcurrentStarts /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart2.ConcurrentStarts]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart2.ConcurrentStarts PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart2.Rejoin /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart2.Rejoin]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart2.Rejoin PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart2.Backup /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart2.Backup]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart2.Backup PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart2.RpcCount /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart2.RpcCount]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart2.RpcCount PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart3.BasicPersist /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart3.BasicPersist]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart3.BasicPersist PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart3.MorePersistence /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart3.MorePersistence]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart3.MorePersistence PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart3.Persist3 /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart3.Persist3]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart3.Persist3 PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart3.UnreliableAgree /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart3.UnreliableAgree]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart3.UnreliableAgree PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart3.Figure8 /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart3.Figure8]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart3.Figure8 PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart3.UnreliableFigure8 /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart3.UnreliableFigure8]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart3.UnreliableFigure8 PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart4.BasicSnapshot /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart4.BasicSnapshot]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart4.BasicSnapshot PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart4.RestoreSnapshot /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart4.RestoreSnapshot]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart4.RestoreSnapshot PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
add_test( RaftTestPart4.OverrideSnapshot /home/stu/chfs/build/test/raft_test [==[--gtest_filter=RaftTestPart4.OverrideSnapshot]==] --gtest_also_run_disabled_tests [==[--gtest_color=auto]==] [==[--gtest_output=xml:/home/stu/chfs/build/test/raft_test.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties( RaftTestPart4.OverrideSnapshot PROPERTIES WORKING_DIRECTORY /home/stu/chfs/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 1000)
set( raft_test_TESTS RaftTestPart1.LeaderElection RaftTestPart1.ReElection RaftTestPart2.BasicAgree RaftTestPart2.FailAgreement RaftTestPart2.FailNoAgreement RaftTestPart2.ConcurrentStarts RaftTestPart2.Rejoin RaftTestPart2.Backup RaftTestPart2.RpcCount RaftTestPart3.BasicPersist RaftTestPart3.MorePersistence RaftTestPart3.Persist3 RaftTestPart3.UnreliableAgree RaftTestPart3.Figure8 RaftTestPart3.UnreliableFigure8 RaftTestPart4.BasicSnapshot RaftTestPart4.RestoreSnapshot RaftTestPart4.OverrideSnapshot)