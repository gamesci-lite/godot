def can_build(env, platform):
    if platform != "web":
        return False
    return env.get("wx_monolith", "no") == "yes"


def configure(env):
    pass


def get_doc_classes():
    return []


def get_doc_path():
    return "doc_classes"