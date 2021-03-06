import os, sys

def insert_dependencies(args, **dependencies):
    result = []
    def add_all(*args):
        for arg in args:
            deps = dependencies.get(arg, [])
            if isinstance(deps, (list, tuple)):
                add_all(*deps)
            else:
                add_all(deps)
            (arg in result) or result.append(arg)

    add_all(*args)
    return result


def check_python(version):
    required_version = tuple(int(i) for i in version.split('.'))
    actual_version = sys.version_info[:2]

    assert actual_version >= required_version, (
        'Must use at least Python %s but have version %d.%d' %
        (version, actual_version[0], actual_version[1]))
