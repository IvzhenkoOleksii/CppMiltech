#!/usr/bin/env bash
unset DEBUGINFOD_URLS
export DEBUGINFOD_URLS=

args=()
for arg in "$@"; do
    case "$arg" in
        '--tty=${DbgTerm}'|'--tty=$DbgTerm')
            tty_path="$(readlink /proc/$$/fd/2 2>/dev/null || true)"
            if [[ -n "$tty_path" && "$tty_path" == /dev/* ]]; then
                args+=("--tty=$tty_path")
            fi
            ;;
        *)
            args+=("$arg")
            ;;
    esac
done

exec /usr/bin/gdb "${args[@]}"
