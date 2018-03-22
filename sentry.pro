TEMPLATE = subdirs

# List all dependencies
SUBDIRS = \
    thirdparty/PDCurses

CONFIG += ordered

# build sentry core eventually
SUBDIRS += sentry_core
