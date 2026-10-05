<p align="right">
  <strong>English</strong> · <a href="README.zh_CN.md">简体中文</a>
</p>

# Coopa game template

Make a game for the Coopa card. This template is a small playable game: Kuba pops out of three holes; move the slipper with ▲▼ and swat with ●.

## Start

1. Click **Use this template** on GitHub to create your own repository.
2. In your repository, open **Code → Codespaces → Create codespace**. The first start takes a few minutes (it installs the SDK).
3. Pick a game id (lowercase letters and digits, starting with a letter, at most 15) and run `coopa rename <your-id>` in the terminal.
4. Run `coopa run`. A browser tab opens with the card screen: ↑↓ are ▲▼, Space is ●.
5. Edit the code and run `coopa run` again.
6. Run `coopa check` for the same check the store runs (boot, screenshots, 5 minutes of random key presses, save size).

Every push also runs the **self-check** workflow (Actions), which additionally builds the card firmware and checks the game size.

You can also open the repository in VS Code with Dev Containers on your own computer (needs Docker).

## Files

| File | What it is |
|---|---|
| `coopa.toml` | The game's ID card: id, name, version, SDK version, entry point, milestones |
| `<id>_texts.h` | All on-screen text (the font is built from these strings) |
| `src/<id>_logic.*` | Game rules: plain C, no screen and no saves |
| `src/<id>_save.*` | Save format and milestones |
| `src/<id>_ui.*` | Drawing (LVGL and `ui_kit.h`) |
| `src/<id>_app.c` | The `kit_app_t` table the card shell calls |

## Rules

The rules are listed in [README.zh_CN.md](README.zh_CN.md); `coopa check` tells you which one a game breaks.

## Submitting

When the Coopa website opens, sign in with GitHub, apply to become a developer, and submit your repository and commit there.
