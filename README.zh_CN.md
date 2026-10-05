<p align="right">
  <a href="README.md">English</a> · <strong>简体中文</strong>
</p>

# Coopa 游戏模板

用这个模板给 Coopa 卡做游戏。里面是一个能玩的小游戏「拍拖鞋」:库巴从三个洞里冒出来,▲▼ 移动拖鞋,● 拍它。

## 开始

1. 在 GitHub 上点 **Use this template**,建一个你自己的仓库。
2. 在你的仓库里点 **Code → Codespaces → Create codespace**。第一次打开要等几分钟(在装 SDK)。
3. 给游戏起个 id(小写字母开头,小写字母和数字,最多 15 个),在终端里运行 `coopa rename 你的id`。
4. 运行 `coopa run`:浏览器会打开一个新标签,里面就是卡的画面。↑↓ 是 ▲▼,空格是 ●。
5. 改代码,再 `coopa run` 看效果。
6. 运行 `coopa check`:跑一遍和商店一样的检查(开机、截图、乱按 5 分钟、存档大小)。

每次推送,「自查」流程(Actions)也会自动跑一遍,还会编卡上的固件、查游戏大小。

也可以在自己电脑上用 VS Code 的 Dev Containers 打开(要装 Docker)。

## 文件夹里有什么

| 文件 | 是什么 |
|---|---|
| `coopa.toml` | 游戏的「身份证」:id、名字、版本、SDK 版本、入口、里程碑 |
| `<id>_texts.h` | 屏幕上的全部文字(字库从这里收集字) |
| `src/<id>_logic.*` | 玩法:纯 C,不碰屏幕和存档 |
| `src/<id>_save.*` | 存档格式和里程碑 |
| `src/<id>_ui.*` | 画面(LVGL 和 `ui_kit.h`) |
| `src/<id>_app.c` | 入口表 `kit_app_t`:卡的外壳通过它叫你的游戏 |

## 规则(不守规则不能上架)

1. **id**:小写字母开头,小写字母和数字,最多 15 个。它也是你的存档名字。
2. **名字**:不是 `static` 的函数和全局变量,名字都以 `你的id_` 开头(上百个游戏编进同一份固件,重名就编不过)。
3. **存档**:只用 `kit_sv_blob_load` / `kit_sv_blob_store` / `kit_sv_ns_erase`,命名空间就是你的 id,最多 2 KB。
4. **文字**:屏幕上的字全写在 `<id>_texts.h` 里,`.c` 里不写中文。字体里没有的字会报错(卡上会显示成空白)。
5. **不许用**:联网、蓝牙、直接读写 NVS / Flash / 分区、重启、睡眠、自己开任务、`EM_ASM` 等嵌 JavaScript、汇编。只用 SDK 给的 `kit_*.h`、`ui_kit.h`、`lvgl.h` 和 C 标准库。
6. **随机数**:在开局时取(`esp_random()`),不在 `init`、`milestones()`、`won()`、`status()` 里取。
7. **里程碑**:最多 8 个,在 `coopa.toml` 里按顺序写,只能往后加。
8. **大小**:游戏在卡上最多占 384 KB。

## 提交

Coopa 官网开放以后:用 GitHub 登录、申请成为开发者,批准后在官网提交你的仓库和 commit。
