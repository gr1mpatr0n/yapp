<p align="center">
<h1>Yapp</h1>
</p>

## A C++ URL parsing library

Make URI handling in C++ easy with a simple compositional syntax. Designed to
be lightweight and similar in syntax to Python's Yarl package.

## Conversion Example

### Python (with Yarl)

```python
url = URL('https://api.github.com') / 'repos' / 'owner' / 'repo'
url = url % {'state': 'open', 'sort': 'created'}
```

### C++ (with Yapp)

```cpp
auto url = URL("https://api.github.com") / "repos" / "owner" / "repo";
url %= {{"state", "open"}, {"sort", "created"}};
```

## Goals

While I have striven to consider performance where appropriate, if performance is
a key consideration for your application, you should consider something like
[Ada](https://github.com/ada-url/ada) instead.