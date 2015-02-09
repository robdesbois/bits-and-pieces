#!/bin/sh

# Usage: shell_ci_on_build_result <dir> <cmd> <args...>
shell_ci_on_build_result()
{
    if [ $# -eq 0 ] ; then
        echo "Usage: $0 <watch-dir> <on-build-cmd> [<args...>]"
		  return 1
    fi

    watchDir="$1"
    shift
    cmd="$1"
    shift
    args="$*"

    [ -e $watchDir/.shell_ci_built ] || touch $watchDir/.shell_ci_built
    [ -e $watchDir/.shell_ci_error ] || touch $watchDir/.shell_ci_error

    echo watching $watchDir/...
    echo will run "$cmd $args"

    # Watch for success & run given command;
    while inotifywait -q -e close_write $watchDir/.shell_ci_built ; do
        date
        $cmd $args || echo -en '\a'
    done &
    successPid=$!

    # Propagate signals to the children
    trap "kill -TERM $successPid ; wait $successPid" TERM INT

    # Watch for failure & notify user
    while inotifywait -q -e close_write $watchDir/.shell_ci_error ; do
        date
        echo "Build failure...."
    done
}


# Usage: shell_ci_build_on_save <dir> <build-cmd> <args...>
# Watches <dir> recursively (ignoring .svn dir) for completion of file write ops.
# When an event happens, <build-cmd> is run with <args...>, and if successful then
# any on_build_result watchers will take the specified action
shell_ci_build_on_save()
{
    if [ $# -eq 0 ] ; then
        echo "Usage: $0 <watch-dir> <build-cmd> [<args...>]"
		  return 1
    fi

    watchDir="$1"
    shift
    cmd="$1"
    shift
    args="$*"

    while inotifywait -r -q -e close_write,moved_to,create $watchDir --exclude='.svn' ; do
        echo running...
        $cmd $args || ( echo error $? && touch .shell_ci_error  && continue )

        echo -e "DONE\n\n\a"
        touch .shell_ci_built
    done
}
