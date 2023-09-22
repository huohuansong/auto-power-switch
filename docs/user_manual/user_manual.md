# Auto Power Switch User Manual

## 1. Introduction

Auto Power Switch is a power sequencer controlled by the command line based on serial port.
The current version supports 3-channels.
Due to the use of small relays, it supports isolated low voltage and low current switching control.
The controlled voltage is usually less than 30V and the current is less than 2A.
It can meet the power timing control of small desktop devices, such as Router, Set-top box or small Switch, etc.

Auto Power Switch supports both manual and automatic control.
You can enter power on or off commands to open or close ports, or bind profiles to ports for automatic execution.
The instructions supported by the profile contain `on`, `off`, `delay`, `goto`, `condition`, and `integer calculation`, etc.
It also supports the saving of profiles, making it easy to continue using them next time.

A typical usage example is shown in the following figure.

![Usage Example](graph/usage_example.svg)

## 2. Command Line Manual

### 2.1 Convention

| Symbol  | Meaning |
| ------- | ------- |
| \<...\> | This is a mandatory field. |
| [...]   | This is a optional field.  |
| \|      | Indicates either one out of two or one out of many. |
| all     | For all. |
| command | Main command word. |
| number  | An integer number. |
| string  | A string. |
|

### 2.2 Command Line Interface

| Command                                             | Meaning |
| ---                                                 | --- |
| help [config \| port \| profile]                    | `help` root command or help for specific commands. |
| config show                                         | Display current configuration. |
| config save                                         | Save current configuration to NVRAM. |
| config clear \<all \| current \| saved\>            | Clear all / current / saved configuration. |
| port [port number] on                               | Open the specified port or all ports. |
| port [port number] off                              | Close the specified port or all ports. |
| port [port number] bind-profile \<profile name\>    | Bind a specified profile to a port or all ports. |
| port [port number] unbind-profile                   | Unbind port or all ports profile. |
| port [port number] active                           | Port needs to be activated after binding the profile. |
| port [port number] status                           | Display the ports current status. |
| profile create \<name\> [max line number]           | Create a profile with a specified name and specify the maximum line number. If max number is omitted, max number defaults to 20. |
| profile show [name]                                 | Display profile content. |
| profile delete \<name\>                             | Delete a profile with a specified name. |
| profile clear \<name\> \<all \| line number\>       | Delete a specified line or all lines of a specified profile. |
| profile modify \<name\> \<line number\> \<string\>  | Modify a specified line of a specified profile. |
| profile check \<name>                               | Check if the profile is legal. |
| profile save [name]                                 | Save a specified profile or all profiles to NVRAM. |
|

### 2.3 Examples

**Example 1: Open all ports manually**
```
> port on
```

**Example 2: Close all ports manually**
```
> port off
```

**Example 3: Open port 1 manually**
```
> port 1 on
```

**Example 4: Display port status**
```
> port status
```

**Example 5: Create a profile called '`twinkle`' for port cycle on and off**
```
> profile create twinkle
> profile modify twinkle 1 "on"
> profile modify twinkle 2 "wait 500"
> profile modify twinkle 3 "off"
> profile modify twinkle 4 "wait 500"
> profile modify twinkle 5 "goto 1"
> profile check twinkle
> profile save twinkle
```
**Example 6: Show profile '`twinkle`' content'**
```
> profile show twinkle
```

**Example 7: All ports bind profile named '`twinkle`' and active**
```
> port bind-profile twinkle
> port active
```

## 3. Profile Design

### 3.1 What is profile

A profile is a rule that can be bound to a specified port to execute according to that rule.

### 3.2 Supported Command Word

| Symbols and Command Words            | Meaning |
| ---                                  | --- |
| var \<variable name\> [= number]     | Define an integer variable called 'variable name'. The name must start with an underline or a letter. The initial value of the variable is 'number'. If omitted, initialize to 0. |
| +                                    | Operator plus. |
| -                                    | Operator minus. |
| *                                    | Operator multiple. |
| /                                    | Operator division. |
| %                                    | Operator mod. |
| on                                   | Power on action. |
| off                                  | Power off action. |
| wait \<variable name \| number\> [s] | Wait for a period of time. The default unit is milliseconds. If 's' are specified, then the unit is second. |
| goto \<line number\>                 | Jump to the specified line to run. |
| if \<condition\>                     | Determine whether to execute based on a condition. The condition include greater than, less than, greater than or equal to, less than or equal to, and equal to between variables, variables and numerical values. |
| else                                 | Conditional statement oprional. Used in conjunction with 'if'. |
| endif                                | Closing statement. Used in conjunction with 'if'. |
| print [variable name \| string] ...  | Display variable or string. |
|

### 3.3 Examples

**Example 1: 1 second cycle on and off**
```
1: on
2: wait 500
3: off
4: wait 500
5: goto 1
```

The timing control of the output is shown in the following figure.

![Profile Example 1](graph/profile_example_1.svg)

**Example 2: Incremental control of power on, with no change in shutdown time**
```
1: var a = 1000
2: on
3: wait a
4: off
5: wait 1000
6: print "a = " a
7: if a < 3000
8: a = a + 1000
9: endif
10: goto 2
```

The timing control of the output is shown in the following figure.

![Profile Example 2](graph/profile_example_2.svg)

## 4. Extension
