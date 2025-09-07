---
tags:
  - plugin
resource_link: "https://www.redguides.com/community/resources/mq2log.140/"
support_link: "https://www.redguides.com/community/threads/mq2log.66845/"
repository: "https://github.com/RedGuides/MQ2Log"
config: "servername_charname.ini"
authors: "Sym, wired420"
tagline: "Plugin writes all mq chat window output to your MQ folder /logs/servername_charname.log"
---

# MQ2Log

<!--desc-start-->
MQ2Log writes all MacroQuest chat window output to your MacroQuest folder at 
`/logs/servername_charname.log`
<!--desc-end-->

## Commands

<a href="cmd-mlog/">
{% 
  include-markdown "projects/mq2log/cmd-mlog.md" 
  start="<!--cmd-syntax-start-->" 
  end="<!--cmd-syntax-end-->" 
%}
</a>
:    {% include-markdown "projects/mq2log/cmd-mlog.md" 
        start="<!--cmd-desc-start-->" 
        end="<!--cmd-desc-end-->" 
        trailing-newlines=false 
     %} {{ readMore('projects/mq2log/cmd-mlog.md') }}

<a href="cmd-mqlogclean/">
{% 
  include-markdown "projects/mq2log/cmd-mqlogclean.md" 
  start="<!--cmd-syntax-start-->" 
  end="<!--cmd-syntax-end-->" 
%}
</a>
:    {% include-markdown "projects/mq2log/cmd-mqlogclean.md" 
        start="<!--cmd-desc-start-->" 
        end="<!--cmd-desc-end-->" 
        trailing-newlines=false 
     %} {{ readMore('projects/mq2log/cmd-mqlogclean.md') }}

<a href="cmd-mqlogcustom/">
{% 
  include-markdown "projects/mq2log/cmd-mqlogcustom.md" 
  start="<!--cmd-syntax-start-->" 
  end="<!--cmd-syntax-end-->" 
%}
</a>
:    {% include-markdown "projects/mq2log/cmd-mqlogcustom.md" 
        start="<!--cmd-desc-start-->" 
        end="<!--cmd-desc-end-->" 
        trailing-newlines=false 
     %} {{ readMore('projects/mq2log/cmd-mqlogcustom.md') }}

## Settings

In your servername_charname.ini file, the MQ2Log settings are kept under the [MQ2Log] section, like this:

```ini
[MQ2Log]
Enabled=1
```

The only setting is "**Enabled**", 1 for ON and 0 for OFF.
