
###############################################################################
#
# Inclusive Makefile for the onlp module.
#
# Autogenerated 2015-03-18 18:40:40.340132
#
###############################################################################
onlp_BASEDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(onlp_BASEDIR)/module/make.mk
include $(onlp_BASEDIR)/module/auto/make.mk
include $(onlp_BASEDIR)/module/src/make.mk
include $(onlp_BASEDIR)/utest/_make.mk

