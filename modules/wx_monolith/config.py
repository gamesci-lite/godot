def can_build(env, platform):
    if platform != "web":
        return False
    # detect.py 用 BoolVariable 注册 wx_monolith, env["wx_monolith"] 是 Python bool 不是字符串.
    # 之前写 == "yes" 永远 False, scons 直接跳过整个 module → SCsub 不跑 → libgd_main_extension.a
    # 没链进 godot.wasm. 修复: 走 bool 真值判断.
    return bool(env.get("wx_monolith", False))


def configure(env):
    pass


def get_doc_classes():
    return []


def get_doc_path():
    return "doc_classes"