# ft_irc

## Build

| Command | Description |
|---|---|
| `make` | Build the server in debug mode (default) |
| `make MODE=release` | Build with optimizations enabled |
| `make re` | Full rebuild (clean + build) |
| `make clean` | Remove compiled object files |
| `make fclean` | Remove object files and the binary |
| `make bonus` | Build with bonus features enabled |

### Source discovery

`srcs.mk` is auto-generated on every build — don't edit it. It lists all `.cpp` files found in `src/`, except `_bonus.cpp` files which are only included with `make bonus`.

> Please run `make fclean` before committing to avoid pushing generated files with stale content.